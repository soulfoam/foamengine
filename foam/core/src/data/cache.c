#include <foam/core/data/cache.h>
#include <foam/core/ext/logger.h>

typedef struct
{
    fe_vec(fe_mesh_bundle_t)   mesh_bundles;
    fe_vec(fe_skeleton_data_t) skeleton_data;
    fe_vec(fe_texture_t)       textures;
    fe_vec(fe_font_t)          fonts;

    fe_hashmap_t mesh_bundle_indexes;
    fe_hashmap_t skeleton_data_indexes;
    fe_hashmap_t texture_indexes;
    fe_hashmap_t font_indexes;

    bool load_on_not_found;

} fe_cache_t;

static fe_cache_t cache;

FE_RETURN_CODE
fe_core_cache_setup(bool load_on_not_found)
{
    FE_RETURN_CODE rc = FE_OK;

    fe_vec_make(cache.mesh_bundles, 100);
    fe_vec_make(cache.skeleton_data, 100);
    fe_vec_make(cache.textures, 100);
    fe_vec_make(cache.fonts, 100);

    fe_hashmap_make(&cache.texture_indexes);
    fe_hashmap_make(&cache.mesh_bundle_indexes);
    fe_hashmap_make(&cache.skeleton_data_indexes);
    fe_hashmap_make(&cache.font_indexes);

    cache.load_on_not_found = load_on_not_found;

    return rc;
}

void
fe_core_cache_cleanup(void)
{
    fe_vec_kill(cache.mesh_bundles,  fe_mesh_bundle_kill);
    fe_vec_kill(cache.skeleton_data, fe_skeleton_data_kill);
    fe_vec_kill(cache.textures,      fe_texture_kill);
    fe_vec_kill(cache.fonts,         fe_font_kill);

    fe_hashmap_kill(&cache.texture_indexes);
    fe_hashmap_kill(&cache.mesh_bundle_indexes);
    fe_hashmap_kill(&cache.skeleton_data_indexes);
    fe_hashmap_kill(&cache.font_indexes);

}

FE_RETURN_CODE
fe_cache_add_mesh_bundle(const char *path)
{
    FE_RETURN_CODE rc = FE_OK;

    fe_mesh_bundle_t *bundle = NULL;
    fe_vec_add_and_get(cache.mesh_bundles, bundle);
    rc = fe_mesh_bundle_make_from_file(bundle, path);

    if (rc == FE_OK)
    {
        fe_hashmap_add(&cache.mesh_bundle_indexes, path, fe_vec_get_last_index(cache.mesh_bundles));
    }

    return rc; 
}

FE_RETURN_CODE
fe_cache_add_skeleton_data(const char *path)
{
    FE_RETURN_CODE rc = FE_OK;

    fe_skeleton_data_t *data = NULL;
    fe_vec_add_and_get(cache.skeleton_data, data);
    rc = fe_skeleton_data_make_from_file(data, path);

    if (rc == FE_OK)
    {
        fe_hashmap_add(&cache.skeleton_data_indexes, path, fe_vec_get_last_index(cache.skeleton_data));
    }

    return rc; 
}

FE_RETURN_CODE
fe_cache_add_texture(const char *path)
{
    FE_RETURN_CODE rc = FE_OK;

    fe_texture_t *texture = NULL;
    fe_vec_add_and_get(cache.textures, texture);
    rc = fe_texture_make_from_file(texture, path);

    if (rc == FE_OK)
    {
        fe_hashmap_add(&cache.texture_indexes, path, fe_vec_get_last_index(cache.textures));
    }

    return rc; 
}

FE_RETURN_CODE
fe_cache_add_font(const char *path, const fe_vec2_t *character_size)
{
    FE_RETURN_CODE rc = FE_OK;

    fe_font_t *font = NULL;
    fe_vec_add_and_get(cache.fonts, font);
    rc = fe_font_make_from_file(font, path, character_size);

    if (rc == FE_OK)
    {
        fe_hashmap_add(&cache.font_indexes, path, fe_vec_get_last_index(cache.fonts));
    }

    return rc; 
}

fe_mesh_bundle_t*
fe_cache_get_mesh_bundle(const char *path)
{
    u64 index = fe_hashmap_get(&cache.mesh_bundle_indexes, path);
    if (index == FE_INVALID_INDEX)
    {
        if (cache.load_on_not_found)
        {
            if (fe_cache_add_mesh_bundle(path) == FE_OK)
            {
                index = fe_hashmap_get(&cache.mesh_bundle_indexes, path);
            }
            else
            {
                return NULL;
            }
        }
    }

    return &cache.mesh_bundles[index];

}

fe_skeleton_data_t*
fe_cache_get_skeleton_data(const char *path)
{
    u64 index = fe_hashmap_get(&cache.skeleton_data_indexes, path);
    if (index == FE_INVALID_INDEX)
    {
        if (cache.load_on_not_found)
        {
            if (fe_cache_add_skeleton_data(path) == FE_OK)
            {
                index = fe_hashmap_get(&cache.skeleton_data_indexes, path);
            }
            else
            {
                return NULL;
            }
        }
    }

    return &cache.skeleton_data[index];
}

fe_texture_t*
fe_cache_get_texture(const char *path)
{
    u64 index = fe_hashmap_get(&cache.texture_indexes, path);
    if (index == FE_INVALID_INDEX)
    {
        if (cache.load_on_not_found)
        {
            if (fe_cache_add_texture(path) == FE_OK)
            {
                index = fe_hashmap_get(&cache.texture_indexes, path);
            }
            else
            {
                return NULL;
            }
        }
    }

    return &cache.textures[index];
}

fe_font_t*
fe_cache_get_font(const char *path)
{
    u64 index = fe_hashmap_get(&cache.font_indexes, path);
    if (index == FE_INVALID_INDEX)
    {
        if (cache.load_on_not_found)
        {
            if (fe_cache_add_font(path, &fe_vec2(8, 16)) == FE_OK)
            {
                index = fe_hashmap_get(&cache.font_indexes, path);
            }
            else
            {
                return NULL;
            }
        }
    }

    return &cache.fonts[index];
}
