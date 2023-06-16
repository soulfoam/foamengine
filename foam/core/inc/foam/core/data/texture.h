#pragma once

#include <foam/core/ext/sugar.h>
#include <foam/core/data/color.h>
#include <foam/core/math/math.h>
#include <foam/core/return_code.h>

FE_DEFINE_HANDLE(fe_texture_id);

typedef struct
fe_texture_t
{
	char *path;

    fe_color_t *pixels;
	fe_vec2_t   size;

	u32 num_of_rows;
} fe_texture_t;

void                fe_texture_make(fe_texture_t *tex, const fe_vec2_t *size);
FE_RETURN_CODE      fe_texture_make_from_file(fe_texture_t *tex, const char *path);
void                fe_texture_make_from_buffer(fe_texture_t *tex, const char *name, const u8 *buffer, size_t buffer_len, const fe_vec2_t *size);
FE_RETURN_CODE      fe_texture_make_atlas(fe_texture_t *tex, const char *path, u32 num_of_rows);
void                fe_texture_kill(fe_texture_t *tex);

void                fe_texture_set_pixel(fe_texture_t *tex, const fe_color_t *pixel, const fe_vec2_t *pos);
fe_color_t          fe_texture_get_pixel(const fe_texture_t *tex, const fe_vec2_t *pos);
void                fe_texture_put_texture(fe_texture_t *target, const fe_texture_t *src, const fe_vec2_t *pos);
const fe_color_t*   fe_texture_get_pixels(const fe_texture_t *tex);
void                fe_texture_clear_pixels(fe_texture_t *tex);

const char*         fe_texture_get_path(const fe_texture_t *tex);
u32                 fe_texture_get_num_of_rows(const fe_texture_t *tex);
fe_vec2_t           fe_texture_get_size(const fe_texture_t *tex);

void                fe_texture_print(const fe_texture_t *tex);
