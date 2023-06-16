#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <foam/core/ext/logger.h>
#include <foam/core/data/stb_image.h>
#include <foam/client/gfx/gl.h>
#include <foam/client/gfx/gpu_texture.h>

static GLuint tex_unit = 0;

void 
fe_gpu_texture_make(fe_gpu_texture_t *gpu_tex, const fe_color_t *pixels, const fe_vec2_t *size, const char *name)
{ 
    gpu_tex->name        = name ? strdup(name) : NULL;
	gpu_tex->unit        = tex_unit;

    gpu_tex->size        = *size;

    gpu_tex->shine_damper = 1.0f;
    gpu_tex->reflectivity = 0.0f;

	glGenTextures(1, &gpu_tex->id);

	fe_gpu_texture_bind(gpu_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
                 gpu_tex->size.x, gpu_tex->size.y, 
                 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	fe_gpu_texture_unbind();
}

void
fe_gpu_texture_make_from_texture(fe_gpu_texture_t *gpu_tex, const fe_texture_t *tex)
{
    fe_gpu_texture_make(gpu_tex, tex->pixels, &tex->size, tex->path);
}

void
fe_gpu_texture_make_fbo(fe_gpu_texture_t *gpu_tex, const fe_vec2_t *size, const char *name)
{
    gpu_tex->name        = name ? strdup(name) : NULL;
	gpu_tex->unit        = tex_unit;

    gpu_tex->size        = *size;

    gpu_tex->shine_damper = 1.0f;
    gpu_tex->reflectivity = 0.0f;

	glGenTextures(1, &gpu_tex->id);

	fe_gpu_texture_bind(gpu_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
                 gpu_tex->size.x, gpu_tex->size.y, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	fe_gpu_texture_unbind();
}


void 
fe_gpu_texture_kill(fe_gpu_texture_t *gpu_tex)
{
    free(gpu_tex->name);
	glDeleteTextures(1, &gpu_tex->id);
}

void
fe_gpu_texture_bind(const fe_gpu_texture_t *gpu_tex)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gpu_tex->id);
}

void
fe_gpu_texture_unbind(void)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void
fe_gpu_texture_upload_pixels(fe_gpu_texture_t *gpu_tex, const fe_color_t *pixels, const fe_vec2_t *size)
{
    gpu_tex->size = *size;
	fe_gpu_texture_bind(gpu_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
                 gpu_tex->size.x, gpu_tex->size.y, 
                 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	fe_gpu_texture_unbind();
}

