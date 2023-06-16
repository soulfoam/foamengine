#pragma once

#include <foam/core/ext/sugar.h>
#include <foam/core/math/math.h>

typedef union 
{
    struct { u8 r, g, b, a; };
    u32 pixel;
} fe_color_t;

#define fe_color_rgba(r, g, b, a) (fe_color_t){{r, g, b, a}}
#define fe_color_rgb(r, g, b) fe_color_rgba(r, g, b, 255)

#define FE_COLOR_TRANSPARENT fe_color_rgba(0, 0, 0, 0)
#define FE_COLOR_WHITE       fe_color_rgba(255, 255, 255, 255)
#define FE_COLOR_BLACK       fe_color_rgba(0, 0, 0, 255)
#define FE_COLOR_RED         fe_color_rgba(255, 0, 0, 255)
#define FE_COLOR_GREEN       fe_color_rgba(0, 255, 0, 255)
#define FE_COLOR_BLUE        fe_color_rgba(0, 0, 255, 255)

static inline void
fe_color_0_to_255(fe_color_t *col)
{
    col->r /= 255;
    col->g /= 255;
    col->b /= 255;
    col->a /= 255;
}

static inline void
fe_color_0_to_1(fe_color_t *col)
{
    col->r *= 255;
    col->g *= 255;
    col->b *= 255;
    col->a *= 255;
}

static inline fe_vec4_t
fe_color_norm(const fe_color_t *col)
{
    return fe_vec4((f32)(col->r/255), (f32)(col->g/255), (f32)(col->b/255), (f32)(col->a/255));
}
