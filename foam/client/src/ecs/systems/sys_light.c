#include <foam/client/ecs/ecs_info.h>
#include <foam/client/gfx/renderer.h>

static const char *S_LIGHT_POS          = "fe_light_position";
static const char *S_LIGHT_COLOR        = "fe_light_color";
static const char *S_LIGHT_ATTENUATION  = "fe_light_attenuation";

void
fe_client_system_light_draw_lights(void)
{
    fe_shader_id shader = fe_renderer_get_shader(fe_renderer_get_active_shader());

    fe_ecs_query(FE_QUERY_LIGHTS, e)
    {
        fe_light_t *light     = fe_entity_get_component(*e, FE_COMPONENT_LIGHT);
        fe_transform_t *xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);
 
        char pos_buffer[64] = {0}, color_buffer[64] = {0}, attenuation_buffer[64] = {0};

        sprintf(pos_buffer,         "%s[%u]", S_LIGHT_POS,         light->gl_index);
        sprintf(color_buffer,       "%s[%u]", S_LIGHT_COLOR,       light->gl_index);
        sprintf(attenuation_buffer, "%s[%u]", S_LIGHT_ATTENUATION, light->gl_index);

        fe_shader_set_vec3(fe_shader_get_location(shader, pos_buffer),         &xform->trl);
        fe_shader_set_vec3(fe_shader_get_location(shader, color_buffer),       &light->color);
        fe_shader_set_vec3(fe_shader_get_location(shader, attenuation_buffer), &light->attenuation);
    }
}


