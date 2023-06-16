#pragma once

#include <foam/core/data/resource.h>
#include <foam/client/gfx/gpu_mesh.h>
#include <foam/client/gfx/gpu_texture.h>

void fe_client_resource_setup(void);
void fe_client_resource_cleanup(void);

fe_gpu_mesh_id           fe_resource_add_gpu_mesh(fe_gpu_mesh_t **tmp_ptr_dst);
void                     fe_resource_del_gpu_mesh(fe_gpu_mesh_id gm_id);
fe_gpu_mesh_t*           fe_resource_get_gpu_mesh(fe_gpu_mesh_id gm_id);
void                     fe_resource_tag_gpu_mesh(fe_gpu_mesh_id gm_id, const char *path, const char *name);
void                     fe_resource_untag_gpu_mesh(fe_gpu_mesh_id gm_id);
fe_gpu_mesh_id           fe_resource_get_gpu_mesh_id(const char *path, const char *name);
fe_genvec(fe_gpu_mesh_t) fe_resource_get_gpu_mesh_genvec(void);
void                     fe_resource_clear_gpu_meshes(void);


fe_gpu_texture_id           fe_resource_add_gpu_texture(fe_gpu_texture_t **tmp_ptr_dst);
void                        fe_resource_del_gpu_texture(fe_gpu_texture_id gt_id);
fe_gpu_texture_t*           fe_resource_get_gpu_texture(fe_gpu_texture_id gt_id);
void                        fe_resource_tag_gpu_texture(fe_gpu_texture_id gt_id, const char *path);
void                        fe_resource_untag_gpu_texture(fe_gpu_texture_id gt_id);
fe_gpu_texture_id           fe_resource_get_gpu_texture_id(const char *path);
fe_genvec(fe_gpu_texture_t) fe_resource_get_gpu_texture_genvec(void);
void                        fe_resource_clear_gpu_textures(void);
