#include <foam/client/ecs/components/fog.h>
#include <foam/client/gfx/shader.h>

static const char *S_HAS_FOG      = "fe_has_fog";
static const char *S_FOG_COLOR    = "fe_fog_color";
static const char *S_FOG_DENSITY  = "fe_fog_density";
static const char *S_FOG_GRADIENT = "fe_fog_gradient";

void 
fe_fog_init(fe_fog_t *fog, const fe_color_t *color, f32 density, f32 gradient)
{
    fog->color = *color;
    fog->density = density;
    fog->gradient = gradient;
}
