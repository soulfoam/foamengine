#pragma once

#include <foam/core/ext/genvec.h>
#include <foam/core/data/texture.h>
#include <foam/core/data/mesh_bundle.h>
#include <foam/core/data/skeleton_data.h>
#include <foam/core/data/font.h>

typedef struct
{
    fe_genvec_id  id;
    char          *path;
    char          *name;
} fe_resource_info_t;

void fe_core_resource_setup(void);
void fe_core_resource_cleanup(void);

//for tagging
void fe_resource_info_make(fe_resource_info_t *info, fe_genvec_id id, const char *path, const char *name);
void fe_resource_info_kill(fe_resource_info_t *info);

fe_mesh_bundle_id           fe_resource_add_mesh_bundle(fe_mesh_bundle_t **tmp_ptr_dst);
void                        fe_resource_del_mesh_bundle(fe_mesh_bundle_id gm_id);
fe_mesh_bundle_t*           fe_resource_get_mesh_bundle(fe_mesh_bundle_id gm_id);
void                        fe_resource_tag_mesh_bundle(fe_mesh_bundle_id gm_id, const char *path, const char *name);
void                        fe_resource_untag_mesh_bundle(fe_mesh_bundle_id gm_id);
fe_mesh_bundle_id           fe_resource_get_mesh_bundle_id(const char *path, const char *name);
fe_genvec(fe_mesh_bundle_t) fe_resource_get_mesh_bundle_genvec(void);
void                    fe_resource_clear_mesh_bundles(void);


fe_skeleton_data_id           fe_resource_add_skeleton_data(fe_skeleton_data_t **tmp_ptr_dst);
void                          fe_resource_del_skeleton_data(fe_skeleton_data_id gt_id);
fe_skeleton_data_t*           fe_resource_get_skeleton_data(fe_skeleton_data_id gt_id);
void                          fe_resource_tag_skeleton_data(fe_skeleton_data_id gt_id, const char *path);
void                          fe_resource_untag_skeleton_data(fe_skeleton_data_id gt_id);
fe_skeleton_data_id           fe_resource_get_skeleton_data_id(const char *path);
fe_genvec(fe_skeleton_data_t) fe_resource_get_skeleton_data_genvec(void);
void                    fe_resource_clear_skeleton_data(void);


fe_texture_id           fe_resource_add_texture(fe_texture_t **tmp_ptr_dst);
void                    fe_resource_del_texture(fe_texture_id gt_id);
fe_texture_t*           fe_resource_get_texture(fe_texture_id gt_id);
void                    fe_resource_tag_texture(fe_texture_id gt_id, const char *path);
void                    fe_resource_untag_texture(fe_texture_id gt_id);
fe_texture_id           fe_resource_get_texture_id(const char *path);
fe_genvec(fe_texture_t) fe_resource_get_texture_genvec(void);
void                    fe_resource_clear_textures(void);

