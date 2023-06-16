#include <foam/core/data/resource.h>

typedef struct
{
    fe_genvec(fe_mesh_bundle_t)   mesh_bundles;
    fe_genvec(fe_skeleton_data_t) skeleton_data;
    fe_genvec(fe_texture_t)       textures;
    fe_genvec(fe_font_t)          fonts;

    fe_vec(fe_resource_info_t) mesh_bundles_info;
    fe_vec(fe_resource_info_t) skeleton_data_info;
    fe_vec(fe_resource_info_t) textures_info;
    fe_vec(fe_resource_info_t) fonts_info;
} fe_resource_manager_t;

static fe_resource_manager_t res;

void
fe_resource_info_make(fe_resource_info_t *info, fe_genvec_id id, const char *path, const char *name)
{
    info->id = id;
    info->path = path ? strdup(path) : NULL;
    info->name = name ? strdup(name) : NULL;
}

void
fe_resource_info_kill(fe_resource_info_t *info)
{
    free(info->path);
    free(info->name);
}

void 
fe_core_resource_setup(void)
{
    res.mesh_bundles  = FE_GENVEC_EMPTY;
    res.skeleton_data = FE_GENVEC_EMPTY;
    res.textures      = FE_GENVEC_EMPTY;
    res.fonts         = FE_GENVEC_EMPTY;

    res.mesh_bundles_info   = FE_VEC_EMPTY;
    res.skeleton_data_info  = FE_VEC_EMPTY;
    res.textures_info       = FE_VEC_EMPTY;
    res.fonts_info          = FE_VEC_EMPTY;
}

void 
fe_core_resource_cleanup(void)
{
    fe_genvec_kill(res.mesh_bundles,  fe_mesh_bundle_kill);
    fe_genvec_kill(res.skeleton_data, fe_skeleton_data_kill);
    fe_genvec_kill(res.textures,      fe_texture_kill);
    fe_genvec_kill(res.fonts,         fe_font_kill);

    fe_vec_kill(res.mesh_bundles_info,   fe_resource_info_kill);
    fe_vec_kill(res.skeleton_data_info,  fe_resource_info_kill);
    fe_vec_kill(res.textures_info,       fe_resource_info_kill);
    fe_vec_kill(res.fonts_info,          fe_resource_info_kill);
}

fe_mesh_bundle_id 
fe_resource_add_mesh_bundle(fe_mesh_bundle_t **tmp_ptr_dst)
{
    fe_mesh_bundle_id gm_id = (fe_mesh_bundle_id){FE_INVALID_ID};
    fe_mesh_bundle_t *mesh_bundle = NULL;
    fe_genvec_add_and_get(res.mesh_bundles, gm_id.id, mesh_bundle);

    if (tmp_ptr_dst) *tmp_ptr_dst = mesh_bundle;

    return gm_id;
}

void              
fe_resource_del_mesh_bundle(fe_mesh_bundle_id gm_id)
{
    if (FE_INVALID_HANDLE_ID(gm_id)) return;

    fe_resource_untag_mesh_bundle(gm_id);
    fe_genvec_del(res.mesh_bundles, gm_id.id, fe_mesh_bundle_kill);
}

fe_mesh_bundle_t* 
fe_resource_get_mesh_bundle(fe_mesh_bundle_id gm_id)
{
    return fe_genvec_get(res.mesh_bundles, gm_id.id);
}

fe_mesh_bundle_id
fe_resource_get_mesh_bundle_id(const char *path, const char *name)
{
    fe_vec_for_each(res.mesh_bundles_info, info)
    {
        if (STR_EQ(info->path, path) && STR_EQ(info->name, name))
        {
            return (fe_mesh_bundle_id){info->id};
        }
    }

    return (fe_mesh_bundle_id){FE_INVALID_ID};
}

void              
fe_resource_tag_mesh_bundle(fe_mesh_bundle_id gm_id, const char *path, const char *name)
{
    fe_mesh_bundle_t *mb = fe_genvec_get(res.mesh_bundles, gm_id.id);
    if (!mb) return;

    fe_resource_info_t *info = NULL;
    fe_vec_add_and_get(res.mesh_bundles_info, info);
    fe_resource_info_make(info, gm_id.id, path, name);
}

void              
fe_resource_untag_mesh_bundle(fe_mesh_bundle_id gm_id)
{
    fe_vec_for_each_index(res.mesh_bundles_info, info, i)
    {
        if (info->id == gm_id.id) 
        {
            fe_vec_del(res.mesh_bundles_info, i, fe_resource_info_kill);
            break;
        }
    }
}

fe_genvec(fe_mesh_bundle_t) 
fe_resource_get_mesh_bundle_genvec(void)
{
    return res.mesh_bundles;
}

void                    
fe_resource_clear_mesh_bundles(void)
{
    fe_genvec_kill(res.mesh_bundles,    fe_mesh_bundle_kill);
    fe_vec_clear(res.mesh_bundles_info, fe_resource_info_kill);
}


fe_skeleton_data_id 
fe_resource_add_skeleton_data(fe_skeleton_data_t **tmp_ptr_dst)
{
    fe_skeleton_data_id sd_id = (fe_skeleton_data_id){FE_INVALID_ID};
    fe_skeleton_data_t *skeleton_data = NULL;
    fe_genvec_add_and_get(res.skeleton_data, sd_id.id, skeleton_data);

    if (tmp_ptr_dst) *tmp_ptr_dst = skeleton_data;

    return sd_id;
}

void              
fe_resource_del_skeleton_data(fe_skeleton_data_id sd_id)
{
    if (FE_INVALID_HANDLE_ID(sd_id)) return;

    fe_resource_untag_skeleton_data(sd_id);
    fe_genvec_del(res.skeleton_data, sd_id.id, fe_skeleton_data_kill);
}

fe_skeleton_data_t* 
fe_resource_get_skeleton_data(fe_skeleton_data_id sd_id)
{
    return fe_genvec_get(res.skeleton_data, sd_id.id);
}

fe_skeleton_data_id
fe_resource_get_skeleton_data_id(const char *path)
{
    fe_vec_for_each(res.skeleton_data_info, info)
    {
        if (STR_EQ(info->path, path))
        {
            return (fe_skeleton_data_id){info->id};
        }
    }

    return (fe_skeleton_data_id){FE_INVALID_ID};
}

void              
fe_resource_tag_skeleton_data(fe_skeleton_data_id sd_id, const char *path)
{
    fe_skeleton_data_t *mb = fe_genvec_get(res.skeleton_data, sd_id.id);
    if (!mb) return;

    fe_resource_info_t *info = NULL;
    fe_vec_add_and_get(res.skeleton_data_info, info);
    fe_resource_info_make(info, sd_id.id, path, NULL);
}

void              
fe_resource_untag_skeleton_data(fe_skeleton_data_id sd_id)
{
    fe_vec_for_each_index(res.skeleton_data_info, info, i)
    {
        if (info->id == sd_id.id) 
        {
            fe_vec_del(res.skeleton_data_info, i, fe_resource_info_kill);
            break;
        }
    }
}

fe_genvec(fe_skeleton_data_t) 
fe_resource_get_skeleton_data_genvec(void)
{
    return res.skeleton_data;
}

void
fe_resource_clear_skeleton_data(void)
{
    fe_genvec_kill(res.skeleton_data, fe_skeleton_data_kill);
    fe_vec_clear(res.skeleton_data_info, fe_resource_info_kill);
}



fe_texture_id 
fe_resource_add_texture(fe_texture_t **tmp_ptr_dst)
{
    fe_texture_id tx_id = (fe_texture_id){FE_INVALID_ID};
    fe_texture_t *texture = NULL;
    fe_genvec_add_and_get(res.textures, tx_id.id, texture);

    if (tmp_ptr_dst) *tmp_ptr_dst = texture;

    return tx_id;
}

void              
fe_resource_del_texture(fe_texture_id tx_id)
{
    if (FE_INVALID_HANDLE_ID(tx_id)) return;

    fe_resource_untag_texture(tx_id);
    fe_genvec_del(res.textures, tx_id.id, fe_texture_kill);
}

fe_texture_t* 
fe_resource_get_texture(fe_texture_id tx_id)
{
    return fe_genvec_get(res.textures, tx_id.id);
}

fe_texture_id
fe_resource_get_texture_id(const char *path)
{
    fe_vec_for_each(res.textures_info, info)
    {
        if (STR_EQ(info->path, path))
        {
            return (fe_texture_id){info->id};
        }
    }

    return (fe_texture_id){FE_INVALID_ID};
}

void              
fe_resource_tag_texture(fe_texture_id tx_id, const char *path)
{
    fe_texture_t *mb = fe_genvec_get(res.textures, tx_id.id);
    if (!mb) return;

    fe_resource_info_t *info = NULL;
    fe_vec_add_and_get(res.textures_info, info);
    fe_resource_info_make(info, tx_id.id, path, NULL);
}

void              
fe_resource_untag_texture(fe_texture_id tx_id)
{
    fe_vec_for_each_index(res.textures_info, info, i)
    {
        if (info->id == tx_id.id) 
        {
            fe_vec_del(res.textures_info, i, fe_resource_info_kill);
            break;
        }
    }
}

fe_genvec(fe_texture_t) 
fe_resource_get_texture_genvec(void)
{
    return res.textures;
}

void                    
fe_resource_clear_textures(void)
{
    fe_genvec_kill(res.textures,    fe_texture_kill);
    fe_vec_clear(res.textures_info, fe_resource_info_kill);
}
