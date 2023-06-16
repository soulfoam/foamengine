#pragma once

#include <foam/core/ext/sugar.h>
#include <foam/core/ext/hashmap.h>
#include <foam/core/data/texture.h>
#include <foam/core/data/mesh_bundle.h>
#include <foam/core/data/skeleton_data.h>
#include <foam/core/ext/vector.h>
#include <foam/core/data/font.h>
#include <foam/core/return_code.h>

FE_RETURN_CODE      fe_core_cache_setup(bool load_on_not_found);
void                fe_core_cache_cleanup(void);
FE_RETURN_CODE      fe_cache_add_texture(const char *path);
FE_RETURN_CODE      fe_cache_add_mesh_bundle(const char *path);
FE_RETURN_CODE      fe_cache_add_skeleton_data(const char *path);
FE_RETURN_CODE      fe_cache_add_font(const char *path, const fe_vec2_t *character_size);

fe_texture_t*       fe_cache_get_texture(const char *path);
fe_mesh_bundle_t*   fe_cache_get_mesh_bundle(const char *path);
fe_skeleton_data_t* fe_cache_get_skeleton_data(const char *path);
fe_font_t*          fe_cache_get_font(const char *path);
