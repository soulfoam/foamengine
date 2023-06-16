#pragma once

#include <foam/client/gfx/gpu_texture.h>
#include <foam/core/return_code.h>
#include <foam/core/ext/sugar.h>

typedef struct
{ 
    fe_gpu_texture_id tex_id;
    fe_color_t        color;
} fe_sprite2d_t;

void fe_sprite2d_init(fe_sprite2d_t *sprite, fe_gpu_texture_id tex_id);
const fe_vec2_t* fe_sprite2d_get_texture_size(fe_sprite2d_t *sprite);
