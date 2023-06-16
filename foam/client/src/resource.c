#include <foam/client/resource.h>

typedef struct
{
    fe_genvec(fe_gpu_mesh_t)    gpu_meshes;
    fe_genvec(fe_gpu_texture_t) gpu_textures;

    fe_vec(fe_resource_info_t) gpu_meshes_info;
    fe_vec(fe_resource_info_t) gpu_textures_info;
} fe_resource_manager_t;

static fe_resource_manager_t res;

void 
fe_client_resource_setup(void)
{
    res.gpu_meshes   = FE_GENVEC_EMPTY;
    res.gpu_textures = FE_GENVEC_EMPTY;

    res.gpu_meshes_info   = FE_VEC_EMPTY;
    res.gpu_textures_info = FE_VEC_EMPTY;
}

void 
fe_client_resource_cleanup(void)
{
    fe_genvec_kill(res.gpu_meshes,    fe_gpu_mesh_kill);
    fe_genvec_kill(res.gpu_textures,  fe_gpu_texture_kill);

    fe_vec_kill(res.gpu_meshes_info,   fe_resource_info_kill);
    fe_vec_kill(res.gpu_textures_info, fe_resource_info_kill);
}


fe_gpu_mesh_id 
fe_resource_add_gpu_mesh(fe_gpu_mesh_t **tmp_ptr_dst)
{
    fe_gpu_mesh_id gm_id = (fe_gpu_mesh_id){FE_INVALID_ID};
    fe_gpu_mesh_t *gpu_mesh = NULL;
    fe_genvec_add_and_get(res.gpu_meshes, gm_id.id, gpu_mesh);

    if (tmp_ptr_dst) *tmp_ptr_dst = gpu_mesh;

    return gm_id;
}

void              
fe_resource_del_gpu_mesh(fe_gpu_mesh_id gm_id)
{
    if (FE_INVALID_HANDLE_ID(gm_id)) return;

    fe_resource_untag_gpu_mesh(gm_id);
    fe_genvec_del(res.gpu_meshes, gm_id.id, fe_gpu_mesh_kill);
}

fe_gpu_mesh_t* 
fe_resource_get_gpu_mesh(fe_gpu_mesh_id gm_id)
{
    return fe_genvec_get(res.gpu_meshes, gm_id.id);
}

fe_gpu_mesh_id
fe_resource_get_gpu_mesh_id(const char *path, const char *name)
{
    fe_vec_for_each(res.gpu_meshes_info, info)
    {
        if (STR_EQ(info->path, path) && STR_EQ(info->name, name))
        {
            return (fe_gpu_mesh_id){info->id};
        }
    }

    return (fe_gpu_mesh_id){FE_INVALID_ID};
}

void              
fe_resource_tag_gpu_mesh(fe_gpu_mesh_id gm_id, const char *path, const char *name)
{
    fe_gpu_mesh_t *mb = fe_genvec_get(res.gpu_meshes, gm_id.id);
    if (!mb) return;

    fe_resource_info_t *info = NULL;
    fe_vec_add_and_get(res.gpu_meshes_info, info);
    fe_resource_info_make(info, gm_id.id, path, name);
}

void              
fe_resource_untag_gpu_mesh(fe_gpu_mesh_id gm_id)
{
    fe_vec_for_each_index(res.gpu_meshes_info, info, i)
    {
        if (info->id == gm_id.id) 
        {
            fe_vec_del(res.gpu_meshes_info, i, fe_resource_info_kill);
            break;
        }
    }
}

fe_genvec(fe_gpu_mesh_t) 
fe_resource_get_gpu_mesh_genvec(void)
{
    return res.gpu_meshes;
}

void                    
fe_resource_clear_gpu_meshes(void)
{
    fe_genvec_kill(res.gpu_meshes,    fe_gpu_mesh_kill);
    fe_vec_clear(res.gpu_meshes_info, fe_resource_info_kill);
}



fe_gpu_texture_id 
fe_resource_add_gpu_texture(fe_gpu_texture_t **tmp_ptr_dst)
{
    fe_gpu_texture_id gt_id = (fe_gpu_texture_id){FE_INVALID_ID};
    fe_gpu_texture_t *gpu_texture = NULL;
    fe_genvec_add_and_get(res.gpu_textures, gt_id.id, gpu_texture);

    if (tmp_ptr_dst) *tmp_ptr_dst = gpu_texture;

    return gt_id;
}

void              
fe_resource_del_gpu_texture(fe_gpu_texture_id gt_id)
{
    if (FE_INVALID_HANDLE_ID(gt_id)) return;

    fe_resource_untag_gpu_texture(gt_id);
    fe_genvec_del(res.gpu_textures, gt_id.id, fe_gpu_texture_kill);
}

fe_gpu_texture_t* 
fe_resource_get_gpu_texture(fe_gpu_texture_id gt_id)
{
    return fe_genvec_get(res.gpu_textures, gt_id.id);
}

fe_gpu_texture_id
fe_resource_get_gpu_texture_id(const char *path)
{
    fe_vec_for_each(res.gpu_textures_info, info)
    {
        if (STR_EQ(info->path, path))
        {
            return (fe_gpu_texture_id){info->id};
        }
    }

    return (fe_gpu_texture_id){FE_INVALID_ID};
}

void              
fe_resource_tag_gpu_texture(fe_gpu_texture_id gt_id, const char *path)
{
    fe_gpu_texture_t *mb = fe_genvec_get(res.gpu_textures, gt_id.id);
    if (!mb) return;

    fe_resource_info_t *info = NULL;
    fe_vec_add_and_get(res.gpu_textures_info, info);
    fe_resource_info_make(info, gt_id.id, path, NULL);
}

void              
fe_resource_untag_gpu_texture(fe_gpu_texture_id gt_id)
{
    fe_vec_for_each_index(res.gpu_textures_info, info, i)
    {
        if (info->id == gt_id.id) 
        {
            fe_vec_del(res.gpu_textures_info, i, fe_resource_info_kill);
            break;
        }
    }
}

fe_genvec(fe_gpu_texture_t) 
fe_resource_get_gpu_texture_genvec(void)
{
    return res.gpu_textures;
}

void                    
fe_resource_clear_gpu_textures(void)
{
    fe_genvec_kill(res.gpu_textures,    fe_gpu_texture_kill);
    fe_vec_clear(res.gpu_textures_info, fe_resource_info_kill);
}



