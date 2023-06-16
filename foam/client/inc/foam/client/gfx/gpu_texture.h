#pragma once

#include <foam/core/data/texture.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/math/math.h>
#include <foam/core/return_code.h>
#include <foam/client/gfx/gl.h>
#include <foam/core/ext/genvec.h>

FE_DEFINE_HANDLE(fe_gpu_texture_id);

typedef struct
fe_gpu_texture_t
{
    char      *name;
	fe_vec2_t size;

	GLuint id;
	GLuint unit;

    f32 shine_damper;
    f32 reflectivity;
} fe_gpu_texture_t;

void fe_gpu_texture_make(fe_gpu_texture_t *gpu_tex, const fe_color_t *pixels, const fe_vec2_t *size, const char *name);
void fe_gpu_texture_make_from_texture(fe_gpu_texture_t *gpu_tex, const fe_texture_t *tex);
void fe_gpu_texture_make_fbo(fe_gpu_texture_t *gpu_tex, const fe_vec2_t *size, const char *name);

void             fe_gpu_texture_kill(fe_gpu_texture_t *gpu_tex);
void             fe_gpu_texture_bind(const fe_gpu_texture_t *gpu_tex);
void             fe_gpu_texture_unbind(void);
void             fe_gpu_texture_upload_pixels(fe_gpu_texture_t *gpu_tex, const fe_color_t *pixels, const fe_vec2_t *size);

//we need set functions im too lazy right now
