#include <foam/client/ecs/ecs_info.h>
#include <foam/client/gfx/renderer.h>

void
fe_client_system_skybox_draw(void)
{ 
    fe_renderer_bind_shader(FE_SHADER_SKYBOX);

    fe_ecs_query(FE_QUERY_SKYBOX, e)
    {
        fe_skybox_t *skybox = fe_entity_get_component(*e, FE_COMPONENT_SKYBOX);

        glDepthFunc(GL_LEQUAL);

        fe_mat4x4_t new_view = *fe_renderer_get_view_matrix(); 
        new_view.raw[3][0] = 0;
        new_view.raw[3][1] = 0;
        new_view.raw[3][2] = 0;
        new_view.raw[3][3] = 0;
        fe_mat4x4_rotate(&new_view, &fe_vec3(1, 0, 0), fe_math_rad(90));
        fe_renderer_push_view_matrix(&new_view);

        glDepthMask(GL_FALSE);
        glBindVertexArray(skybox->vao);
        fe_skybox_bind(skybox);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
        fe_skybox_unbind();

        glDepthFunc(GL_LESS);

        fe_renderer_pop_view_matrix(); 
    }
}

