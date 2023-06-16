#include <string.h>
#include <foam/client/gfx/shader.h>
#include <foam/client/gfx/gpu_mesh.h>
#include <foam/client/gfx/gpu_texture.h>
#include <foam/client/resource.h>
#include <foam/client/gfx/renderer.h>
#include <foam/core/math/math.h>
#include <foam/core/ext/logger.h>
#include <foam/core/ext/vector.h>

static const char *S_HAS_TEXTURE          = "fe_has_texture";
static const char *S_TEXTURE_SHINE_DAMPER = "fe_texture_shine_damper";
static const char *S_TEXTURE_REFLECTIVITY = "fe_texture_reflectivity";

void
fe_gpu_mesh_make(fe_gpu_mesh_t *gpu_mesh)
{ 
	gpu_mesh->vert_count       = 0;
	gpu_mesh->indice_count     = 0;
	gpu_mesh->instances        = FE_GENVEC_EMPTY;
	gpu_mesh->tex_id           = (fe_gpu_texture_id){FE_INVALID_ID};
	gpu_mesh->tex_index        = 0;
	gpu_mesh->render_flags     = 0;

	glGenVertexArrays(1, &gpu_mesh->vao);
    glGenBuffers(1, &gpu_mesh->vbo);
    glGenBuffers(1, &gpu_mesh->ebo);
    glGenBuffers(1, &gpu_mesh->instance_vbo);
}

void
fe_gpu_mesh_make_from_mesh(fe_gpu_mesh_t *gpu_mesh, const fe_mesh_t *mesh)
{
    fe_gpu_mesh_make(gpu_mesh);
    fe_gpu_mesh_bind_buffer(gpu_mesh, mesh->verts, fe_vec_get_count(mesh->verts), mesh->indices, fe_vec_get_count(mesh->indices));
}

void
fe_gpu_mesh_kill(fe_gpu_mesh_t *gpu_mesh)
{
    fe_genvec_kill(gpu_mesh->instances, NULL);
	glDeleteBuffers(1, &gpu_mesh->vbo);
	glDeleteBuffers(1, &gpu_mesh->ebo);
	glDeleteBuffers(1, &gpu_mesh->instance_vbo);
	glDeleteVertexArrays(1, &gpu_mesh->vao);
}

fe_gpu_mesh_instance_id
fe_gpu_mesh_add_instance(fe_gpu_mesh_t *gpu_mesh, const fe_mat4x4_t *xform, const fe_color_t *color, const fe_vec2_t *tex_offset)
{
    fe_gpu_mesh_instance_t *ins    = NULL;
    fe_gpu_mesh_instance_id ins_id = (fe_gpu_mesh_instance_id){FE_INVALID_ID};
    fe_genvec_add_and_get(gpu_mesh->instances, ins_id.id, ins);
    ins->matrix     = *xform;
    ins->color      = *color;
    ins->tex_offset = *tex_offset;

    return ins_id;
}

void                   
fe_gpu_mesh_del_instance(fe_gpu_mesh_t *gpu_mesh, fe_gpu_mesh_instance_id ins_id)
{
    fe_genvec_del(gpu_mesh->instances, ins_id.id, NULL);
}

void      
fe_gpu_mesh_update_instance_matrix(fe_gpu_mesh_t *gpu_mesh, 
                            fe_gpu_mesh_instance_id ins_id, 
                            const fe_mat4x4_t *mat)
{
    fe_gpu_mesh_instance_t *ins = fe_genvec_get(gpu_mesh->instances, ins_id.id);
    if (!ins) return;

    ins->matrix = *mat;
}

void      
fe_gpu_mesh_update_instance_color(fe_gpu_mesh_t *gpu_mesh, 
                                  fe_gpu_mesh_instance_id ins_id, 
                                  const fe_color_t *color)
{
    fe_gpu_mesh_instance_t *ins = fe_genvec_get(gpu_mesh->instances, ins_id.id);
    if (!ins) return;

    ins->color = *color;
}

void      
fe_gpu_mesh_update_instance_texture_offset(fe_gpu_mesh_t *gpu_mesh, 
                                           fe_gpu_mesh_instance_id ins_id, 
                                           const fe_vec2_t *tex_offset)
{
    fe_gpu_mesh_instance_t *ins = fe_genvec_get(gpu_mesh->instances, ins_id.id);
    if (!ins) return;

    ins->tex_offset = *tex_offset;
}

fe_gpu_mesh_instance_t*
fe_gpu_mesh_get_instance(const fe_gpu_mesh_t *gpu_mesh, fe_gpu_mesh_instance_id ins_id)
{
    return fe_genvec_get(gpu_mesh->instances, ins_id.id);
}

u32                     
fe_gpu_mesh_get_instance_count(const fe_gpu_mesh_t *gpu_mesh)
{
    return fe_genvec_get_count(gpu_mesh->instances);
}

void
fe_gpu_mesh_upload_instances(fe_gpu_mesh_t *gpu_mesh)
{
	glBindVertexArray(gpu_mesh->vao);

    glBindBuffer(GL_ARRAY_BUFFER, gpu_mesh->instance_vbo);
    glBufferData
    (
        GL_ARRAY_BUFFER, 
        fe_genvec_get_count(gpu_mesh->instances) * sizeof(fe_gpu_mesh_instance_t), 
        gpu_mesh->instances, 
        GL_STREAM_DRAW
    );

    glEnableVertexAttribArray(3); 
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(fe_gpu_mesh_instance_t), (void*)0);

    glEnableVertexAttribArray(4); 
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(fe_gpu_mesh_instance_t), (void*)(1 * sizeof(fe_vec4_t)));

    glEnableVertexAttribArray(5); 
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(fe_gpu_mesh_instance_t), (void*)(2 * sizeof(fe_vec4_t)));	

    glEnableVertexAttribArray(6); 
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(fe_gpu_mesh_instance_t), (void*)(3 * sizeof(fe_vec4_t)));	

    glEnableVertexAttribArray(7); 
    glVertexAttribPointer(7, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(fe_gpu_mesh_instance_t), (void*)(4 * sizeof(fe_vec4_t)));

    glEnableVertexAttribArray(8); 
    glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, sizeof(fe_gpu_mesh_instance_t), (void*)((4 * sizeof(fe_vec4_t)) + (sizeof(u8) * 4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);
    glVertexAttribDivisor(8, 1);

    glBindVertexArray(0);
}

void
fe_gpu_mesh_bind_buffer(fe_gpu_mesh_t *gpu_mesh, const fe_vertex_t *verts, u64 vert_count, const u32 *indices, u64 indice_count)
{
    gpu_mesh->vert_count   = vert_count;
    gpu_mesh->indice_count = indice_count;

    glBindVertexArray(gpu_mesh->vao);

    glBindBuffer(GL_ARRAY_BUFFER, gpu_mesh->vbo);
    glBufferData(
			GL_ARRAY_BUFFER, 
			FE_VERTS_IN_BYTES(gpu_mesh->vert_count), 
			verts, 
			GL_STREAM_DRAW);

	u32 slot = 0;
	glVertexAttribPointer(slot++, 3, GL_FLOAT, GL_FALSE, sizeof(fe_vertex_t), (void*)0);
	glVertexAttribPointer(slot++, 2, GL_FLOAT, GL_FALSE, sizeof(fe_vertex_t), (void*)(3 * sizeof(f32)));
	glVertexAttribPointer(slot++, 3, GL_FLOAT, GL_FALSE, sizeof(fe_vertex_t), (void*)(5 * sizeof(f32)));	

	for (u32 i = 0; i < slot; i++) glEnableVertexAttribArray(i);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu_mesh->ebo);
    glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, 
			FE_INDICES_IN_BYTES(gpu_mesh->indice_count), 
			indices, 
			GL_STREAM_DRAW);

	glBindVertexArray(0);

}

static void
bind_render_flags(fe_gpu_mesh_t *gpu_mesh)
{
	if (gpu_mesh->render_flags & FE_GPU_MESH_RENDER_TRANSPARENT)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	if (gpu_mesh->render_flags & FE_GPU_MESH_RENDER_DISABLE_CULLING) 
    {
        glDisable(GL_CULL_FACE);
    }

    if (gpu_mesh->render_flags & FE_GPU_MESH_RENDER_WIREFRAME)
    { 
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    
}

static void
unbind_render_flags(fe_gpu_mesh_t *gpu_mesh)
{	
	if (gpu_mesh->render_flags & FE_GPU_MESH_RENDER_TRANSPARENT)
	{
		glDisable(GL_BLEND);
	}

	if (gpu_mesh->render_flags & FE_GPU_MESH_RENDER_DISABLE_CULLING)
    {
        glEnable(GL_CULL_FACE);
    }

    if (gpu_mesh->render_flags & FE_GPU_MESH_RENDER_WIREFRAME)
    { 
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

}

void
fe_gpu_mesh_draw(fe_gpu_mesh_t *gpu_mesh, fe_shader_id shader)
{
	if (fe_genvec_get_count(gpu_mesh->instances) <= 0) return;

    fe_gpu_texture_t *tex = fe_resource_get_gpu_texture(gpu_mesh->tex_id);

    if (tex)
    {
        fe_shader_set_bool(fe_shader_get_location(shader, S_HAS_TEXTURE), true);
        fe_shader_set_f32(fe_shader_get_location(shader, S_TEXTURE_SHINE_DAMPER), tex->shine_damper);
        fe_shader_set_f32(fe_shader_get_location(shader, S_TEXTURE_REFLECTIVITY), tex->reflectivity);
    }
    else
    {
        fe_shader_set_bool(fe_shader_get_location(shader, S_HAS_TEXTURE), false);
    }

	bind_render_flags(gpu_mesh);

	glBindVertexArray(gpu_mesh->vao);

	if (tex) fe_gpu_texture_bind(tex);

    glDrawElementsInstanced(GL_TRIANGLES, 
        gpu_mesh->indice_count, GL_UNSIGNED_INT, 
        0,
        fe_genvec_get_count(gpu_mesh->instances));

	if (tex) fe_gpu_texture_unbind();

	glBindVertexArray(0);

	unbind_render_flags(gpu_mesh);
}

void
fe_gpu_mesh_set_texture_id(fe_gpu_mesh_t *gpu_mesh, fe_gpu_texture_id tex_id)
{
    gpu_mesh->tex_id = tex_id;
}

fe_gpu_texture_id
fe_gpu_mesh_get_texture_id(const fe_gpu_mesh_t *gpu_mesh)
{
    return gpu_mesh->tex_id;
}

bool           
fe_gpu_mesh_get_render_flag(const fe_gpu_mesh_t *gpu_mesh, FE_GPU_MESH_RENDER_FLAG flag)
{
    return flag & gpu_mesh->render_flags;
}

void           
fe_gpu_mesh_set_render_flags(fe_gpu_mesh_t *gpu_mesh, u32 flags)
{
    gpu_mesh->render_flags = flags;
}

void           
fe_gpu_mesh_enable_render_flag(fe_gpu_mesh_t *gpu_mesh, FE_GPU_MESH_RENDER_FLAG flag)
{
    gpu_mesh->render_flags |= flag;
}

void           
fe_gpu_mesh_disable_render_flag(fe_gpu_mesh_t *gpu_mesh, FE_GPU_MESH_RENDER_FLAG flag)
{
    gpu_mesh->render_flags &= ~flag;
}

