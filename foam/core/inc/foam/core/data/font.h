#pragma once

#include <stdio.h>
#include <foam/core/ext/logger.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/io.h>
#include <foam/core/ext/hashmap.h>
#include <foam/core/data/texture.h>

FE_DEFINE_HANDLE(fe_font_id);

typedef struct
{
    fe_reader_t  data;
    fe_vec2_t    character_size;

} fe_font_t;

void           fe_font_make_from_buffer(fe_font_t *font, const u8 *buffer, size_t buf_len, const fe_vec2_t *character_size);
FE_RETURN_CODE fe_font_make_from_file(fe_font_t *font, const char *path, const fe_vec2_t *character_size);
fe_texture_t   fe_font_make_texture(const fe_font_t *font, const char *str, const fe_vec2_t *texture_size, const fe_color_t *fg_color, const fe_color_t *bg_color);
void           fe_font_print_on_texture(const fe_font_t *font, fe_texture_t *tex, const char *str, const fe_vec2_t *pos, const fe_color_t *fg, const fe_color_t *bg);
void           fe_font_kill(fe_font_t *font);
