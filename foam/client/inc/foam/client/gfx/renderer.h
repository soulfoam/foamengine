#pragma once

#include <foam/core/return_code.h>
#include <foam/core/data/mesh_bundle.h>
#include <foam/core/ext/genvec.h>
#include <foam/client/gfx/shader.h>
#include <foam/client/gfx/gpu_mesh.h>
#include <foam/client/gfx/gpu_texture.h>

FE_RETURN_CODE    fe_renderer_setup(const fe_vec2_t *render_resolution, u32 shader_count);
void              fe_renderer_cleanup(void);

FE_RETURN_CODE    fe_renderer_register_shader(FE_SHADER_TYPE type, const char *path);
void              fe_renderer_draw_gpu_mesh(fe_gpu_mesh_t *gpu_mesh);
void              fe_renderer_draw_resource_gpu_meshes(void);
void              fe_renderer_draw_gpu_texture(fe_gpu_texture_t *tex, 
                                             const fe_vec2_t *pos, const fe_quat_t *rot, const fe_vec2_t *size, 
                                             const fe_color_t *color);

fe_shader_id   fe_renderer_get_shader(FE_SHADER_TYPE type);
void           fe_renderer_bind_shader(FE_SHADER_TYPE type);
FE_SHADER_TYPE fe_renderer_get_active_shader(void);

void        fe_renderer_push_view_matrix(const fe_mat4x4_t *src);
void        fe_renderer_pop_view_matrix(void);
const fe_mat4x4_t* fe_renderer_get_view_matrix(void);

void        fe_renderer_push_projection_matrix(const fe_mat4x4_t *src);
void        fe_renderer_pop_projection_matrix(void);
const fe_mat4x4_t* fe_renderer_get_projection_matrix(void);

void       fe_renderer_set_clear_color(const fe_color_t *color);
const fe_color_t* fe_renderer_get_clear_color(void);

void  fe_renderer_start_draw(void);
void  fe_renderer_end_draw(void);
void  fe_renderer_render(void);
void  fe_renderer_set_resolution(const fe_vec2_t *size);
const fe_vec2_t* fe_renderer_get_resolution(void);
