#pragma once

#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/return_code.h>
#include <foam/core/data/texture.h>

typedef struct
{
    fe_color_t color;
    f32        density;
    f32        gradient;
} fe_fog_t;

void fe_fog_init(fe_fog_t *fog, const fe_color_t *color, f32 density, f32 gradient);
