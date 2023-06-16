#pragma once

#include <foam/client/gfx/gl.h>
#include <foam/client/gfx/shader_source.h>
#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/return_code.h>

typedef GLuint fe_shader_id;
typedef GLint fe_shader_get_location_id;

typedef enum
{
    FE_SHADER_INSTANCING, 
    FE_SHADER_SKYBOX,
    FE_SHADER_UI,

    FE_SHADER_COUNT
} FE_SHADER_TYPE;

FE_RETURN_CODE        fe_shader_make(fe_shader_id *shader, fe_shader_source_t *src);
void                  fe_shader_kill(fe_shader_id shader);
void                  fe_shader_use(fe_shader_id shader);
fe_shader_get_location_id fe_shader_get_location(fe_shader_id shader, const char *name);
void                  fe_shader_set_bool(fe_shader_get_location_id location, bool value);
void                  fe_shader_set_int(fe_shader_get_location_id location, int value);
void                  fe_shader_set_f32(fe_shader_get_location_id location, f32 value);
void                  fe_shader_set_vec2(fe_shader_get_location_id location, const fe_vec2_t *value);
void                  fe_shader_set_vec3(fe_shader_get_location_id location, const fe_vec3_t *value);
void                  fe_shader_set_vec4(fe_shader_get_location_id location, const fe_vec4_t *value);
void                  fe_shader_set_mat4x4(fe_shader_get_location_id location, const fe_mat4x4_t *value);

