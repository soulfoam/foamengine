#pragma once

#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/return_code.h>

#define FE_MAX_LIGHTS 5

typedef struct
{
    u32 gl_index;
	fe_vec3_t color;
    fe_vec3_t attenuation;

} fe_light_t;

FE_RETURN_CODE fe_light_make(fe_light_t *light, const fe_vec3_t *color);
void           fe_light_kill(fe_light_t *light);
