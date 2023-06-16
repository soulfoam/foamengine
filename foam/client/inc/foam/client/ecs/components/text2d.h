#pragma once

#include <foam/core/return_code.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/data/font.h>
#include <foam/client/gfx/gpu_texture.h>

typedef struct
{ 
    const fe_font_t   *font; 

    fe_texture_t      tex;
    fe_gpu_texture_t  gpu_tex;
    
    fe_color_t fg_color;
    fe_color_t bg_color;

    bool dirty;

} fe_text2d_t;

void fe_text2d_make(fe_text2d_t *text, const fe_font_t *font, const fe_vec2_t *tex_size, const fe_color_t *fg_color, const fe_color_t *bg_color);
void fe_text2d_kill(fe_text2d_t *text);
void fe_text2d_resize_texture(fe_text2d_t *text, const fe_vec2_t *size);
void fe_text2d_print(fe_text2d_t *text, const char *str, const fe_vec2_t *pos);
void fe_text2d_clear(fe_text2d_t *text);

