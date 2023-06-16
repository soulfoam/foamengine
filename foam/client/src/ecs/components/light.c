#include <foam/client/ecs/components/light.h>
#include <foam/client/gfx/shader.h>
#include <foam/client/gfx/renderer.h>

static const char *S_LIGHT_POS          = "fe_light_position";
static const char *S_LIGHT_COLOR        = "fe_light_color";
static const char *S_LIGHT_ATTENUATION  = "fe_light_attenuation";
static bool in_use[FE_MAX_LIGHTS];

FE_RETURN_CODE 
fe_light_make(fe_light_t *light, const fe_vec3_t *color)
{
    light->color = *color;
	light->attenuation = fe_vec3(1, 0, 0);

    for (u32 i = 0; i < FE_MAX_LIGHTS; i++)
    {
        if (!in_use[i])
        {
            light->gl_index = i;
            in_use[i] = true;
            break;
        }
    }

    return FE_OK;
}

void
fe_light_kill(fe_light_t *light)
{
    in_use[light->gl_index] = false;
}


