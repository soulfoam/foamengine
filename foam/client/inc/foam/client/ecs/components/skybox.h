#pragma once

#include <foam/core/return_code.h>
#include <foam/core/data/mesh_bundle.h>
#include <foam/core/ext/sugar.h>
#include <foam/client/gfx/gl.h>

typedef struct
{ 
	GLuint id;
	GLuint vao;
	GLuint vbo;

    fe_mesh_t mesh;

} fe_skybox_t;

FE_RETURN_CODE fe_skybox_make(fe_skybox_t *skybox, u32 texture_count, const char **textures);
void           fe_skybox_kill(fe_skybox_t *sky);
void           fe_skybox_bind(const fe_skybox_t *skybox);
void           fe_skybox_unbind(void);
