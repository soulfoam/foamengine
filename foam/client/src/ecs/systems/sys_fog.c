#include <foam/client/ecs/ecs_info.h>
#include <foam/client/gfx/renderer.h>

static const char *S_HAS_FOG      = "fe_has_fog";
static const char *S_FOG_COLOR    = "fe_fog_color";
static const char *S_FOG_DENSITY  = "fe_fog_density";
static const char *S_FOG_GRADIENT = "fe_fog_gradient";

void
fe_client_system_fog_draw(void)
{
    fe_shader_id shader = fe_renderer_get_shader(fe_renderer_get_active_shader());

    fe_ecs_query(FE_QUERY_FOG, e)
    {
        fe_fog_t *fog = fe_entity_get_component(*e, FE_COMPONENT_FOG);
        
        fe_shader_set_vec3(fe_shader_get_location(shader, S_FOG_COLOR),    &fe_vec3(fog->color.r, fog->color.g, fog->color.b));
        fe_shader_set_f32(fe_shader_get_location(shader,  S_FOG_DENSITY),  fog->density);
        fe_shader_set_f32(fe_shader_get_location(shader,  S_FOG_GRADIENT), fog->gradient);
        fe_shader_set_bool(fe_shader_get_location(shader, S_HAS_FOG),      true);
    }
}

