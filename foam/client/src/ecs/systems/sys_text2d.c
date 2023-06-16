#include <foam/client/ecs/ecs_info.h>
#include <foam/client/gfx/window.h>
#include <foam/client/gfx/renderer.h>

void 
fe_client_system_text2d_draw(void)
{
    fe_renderer_bind_shader(FE_SHADER_UI);

    const fe_vec2_t *reso  = fe_renderer_get_resolution();
    fe_mat4x4_t projection = fe_mat4x4_ortho(0.0f, reso->x, reso->y, 0.0f, -1.0f, 1.0f);
    fe_renderer_push_projection_matrix(&projection);

    fe_ecs_query(FE_QUERY_TEXT2D, e)
    {
        fe_text2d_t *text     = fe_entity_get_component(*e, FE_COMPONENT_TEXT2D);
        fe_transform_t *xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);

        if (text->dirty)
        {
            fe_gpu_texture_upload_pixels(&text->gpu_tex, text->tex.pixels, &text->tex.size);
            text->dirty = false;
        }

        fe_renderer_draw_gpu_texture
        (
            &text->gpu_tex, 
            &fe_vec2(xform->trl.x, xform->trl.y), 
            &xform->rot, 
            &fe_vec2(xform->scl.x, xform->scl.y), 
            &FE_COLOR_WHITE
        );

    }

    fe_renderer_pop_projection_matrix();
}

