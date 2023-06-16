#pragma once

#include <foam/core/ext/sugar.h>
#include <foam/core/ext/genvec.h>
#include <foam/core/ext/value.h>
#include <foam/core/return_code.h>
#include <foam/core/data/mesh_bundle.h>
#include <foam/client/gfx/gl.h>
#include <foam/client/gfx/shader.h>
#include <foam/client/gfx/gpu_texture.h>

FE_DEFINE_HANDLE(fe_gpu_mesh_id);
FE_DEFINE_HANDLE(fe_gpu_mesh_instance_id);

typedef enum
{
	FE_GPU_MESH_RENDER_OUTLINE         = 1 << 0,
	FE_GPU_MESH_RENDER_TRANSPARENT     = 1 << 1,
	FE_GPU_MESH_RENDER_DISABLE_CULLING = 1 << 2, 
	FE_GPU_MESH_RENDER_WIREFRAME       = 1 << 3,


	FE_GPU_MESH_RENDER_FLAG_COUNT
} FE_GPU_MESH_RENDER_FLAG;

typedef enum
{
    FE_GPU_MESH_DRAW_STATIC,
    FE_GPU_MESH_DRAW_STREAM,
} FE_GPU_MESH_DRAW_TYPE;

typedef struct
{
    fe_mat4x4_t matrix;
    fe_color_t  color;
    fe_vec2_t   tex_offset;
} fe_gpu_mesh_instance_t;

typedef struct
{
	u64 vert_count;
	u64 indice_count;

	fe_gpu_texture_id tex_id;
	u32               tex_index;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint instance_vbo;

	u32                   render_flags;
    FE_GPU_MESH_DRAW_TYPE draw_type;

    fe_genvec(fe_gpu_mesh_instance_t) instances;
} fe_gpu_mesh_t;

void           fe_gpu_mesh_make(fe_gpu_mesh_t *gpu_mesh);
void           fe_gpu_mesh_make_from_mesh(fe_gpu_mesh_t *gpu_mesh, const fe_mesh_t *mesh);
void           fe_gpu_mesh_kill(fe_gpu_mesh_t *gpu_mesh);

void           fe_gpu_mesh_bind_buffer(fe_gpu_mesh_t *gpu_mesh, const fe_vertex_t *verts, u64 vert_count, const u32 *indices, u64 indice_count);
void           fe_gpu_mesh_upload_instances(fe_gpu_mesh_t *gpu_mesh);
void           fe_gpu_mesh_draw(fe_gpu_mesh_t *gpu_mesh, fe_shader_id shader);
fe_gpu_mesh_instance_id fe_gpu_mesh_add_instance(fe_gpu_mesh_t *gpu_mesh, const fe_mat4x4_t *xform, const fe_color_t *color, const fe_vec2_t *tex_offset);
void                    fe_gpu_mesh_del_instance(fe_gpu_mesh_t *gpu_mesh, fe_gpu_mesh_instance_id ins_id);
void                    fe_gpu_mesh_update_instance_matrix(fe_gpu_mesh_t *gpu_mesh, 
                                                           fe_gpu_mesh_instance_id ins_id, 
                                                           const fe_mat4x4_t *mat);
void                    fe_gpu_mesh_update_instance_color(fe_gpu_mesh_t *gpu_mesh, 
                                                          fe_gpu_mesh_instance_id ins_id, 
                                                          const fe_color_t *color);
void                    fe_gpu_mesh_update_instance_texture_offset(fe_gpu_mesh_t *gpu_mesh, 
                                                                   fe_gpu_mesh_instance_id ins_id, 
                                                                   const fe_vec2_t *tex_offset);
u32                     fe_gpu_mesh_get_instance_count(const fe_gpu_mesh_t *gpu_mesh);
fe_gpu_mesh_instance_t *fe_gpu_mesh_get_instance(const fe_gpu_mesh_t *gpu_mesh, fe_gpu_mesh_instance_id ins_id);

void              fe_gpu_mesh_set_texture_id(fe_gpu_mesh_t *gpu_mesh, fe_gpu_texture_id tex_id);
fe_gpu_texture_id fe_gpu_mesh_get_texture_id(const fe_gpu_mesh_t *gpu_mesh);
bool              fe_gpu_mesh_get_render_flag(const fe_gpu_mesh_t *gpu_mesh, FE_GPU_MESH_RENDER_FLAG flag);
void              fe_gpu_mesh_set_render_flags(fe_gpu_mesh_t *gpu_mesh, u32 flags);
void              fe_gpu_mesh_enable_render_flag(fe_gpu_mesh_t *gpu_mesh, FE_GPU_MESH_RENDER_FLAG flag);
void              fe_gpu_mesh_disable_render_flag(fe_gpu_mesh_t *gpu_mesh, FE_GPU_MESH_RENDER_FLAG flag);

