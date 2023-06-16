#include <foam/client/ecs/ecs_info.h>
#include <foam/client/resource.h>
#include <foam/client/gfx/window.h>
#include <foam/client/gfx/renderer.h>

void 
fe_client_system_sprite2d_draw(void)
{
    fe_renderer_bind_shader(FE_SHADER_UI);

    const fe_vec2_t *reso  = fe_renderer_get_resolution();
    fe_mat4x4_t projection = fe_mat4x4_ortho(0.0f, reso->x, reso->y, 0.0f, -1.0f, 1.0f);
    fe_renderer_push_projection_matrix(&projection);

    fe_ecs_query(FE_QUERY_SPRITE2D, e)
    {
        fe_sprite2d_t *sprite = fe_entity_get_component(*e, FE_COMPONENT_SPRITE2D);
        fe_transform_t *xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);

        fe_gpu_texture_t *tex = fe_resource_get_gpu_texture(sprite->tex_id);
        fe_renderer_draw_gpu_texture(
                    tex, 
                    &fe_vec2(xform->trl.x, xform->trl.y), 
                    &xform->rot, 
                    &fe_vec2(xform->scl.x, xform->scl.y), 
                    &sprite->color);

    }

    fe_renderer_pop_projection_matrix();

}

