#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <foam/client/gfx/gl.h>
#include <foam/client/gfx/shader.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/logger.h>
#include <foam/core/ext/string.h>

static FE_RETURN_CODE
err_check(const fe_shader_source_t *src, GLuint id, const char *type)
{
    FE_RETURN_CODE rc = FE_OK;

    const char *path = src->path ? src->path : "";
	GLint success;
	GLchar log[1024];
	if(STR_NOT_EQ(type, "PROGRAM"))
	{
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(id, 1024, NULL, log); 
            rc = FE_ERR_GL_SHADER_COMPILE;
			fe_log_return_code(rc, "'%s' %s compile error: \n%s\n", path, type, log);
		}
	}
	else
	{
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if(!success)
		{
			glGetProgramInfoLog(id, 1024, NULL, log);
            rc = FE_ERR_GL_SHADER_LINK;
			fe_log_return_code(rc, "'%s' %s linking error: \n%s\n", path, type, log);
		}
	}

    return rc;
}

FE_RETURN_CODE
fe_shader_make(fe_shader_id *shader, fe_shader_source_t *src)
{
    FE_RETURN_CODE rc = FE_OK;

	GLuint v_id = 0;
	GLuint f_id = 0;		

	v_id = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(v_id, 1, (const char**)&src->vertex_data.buffer, NULL);
	glCompileShader(v_id);
	rc = err_check(src, v_id, "VERTEX");

	f_id = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(f_id, 1, (const char**)&src->fragment_data.buffer, NULL);
	glCompileShader(f_id);
	rc = err_check(src, f_id, "FRAGMENT");

	GLuint id = glCreateProgram();
	glAttachShader(id, v_id);
	glAttachShader(id, f_id);

	glLinkProgram(id);
	rc = err_check(src, id, "PROGRAM");
    if (rc == FE_OK) *shader = id;

	glDeleteShader(v_id);
	glDeleteShader(f_id);

    return rc;
}

void
fe_shader_kill(fe_shader_id shader)
{
	glDeleteProgram(shader);
}

void
fe_shader_use(fe_shader_id shader)
{
	glUseProgram(shader);
}

fe_shader_get_location_id
fe_shader_get_location(fe_shader_id shader, const char *name)
{
    return glGetUniformLocation(shader, name);
}

void 
fe_shader_set_bool(fe_shader_get_location_id location, bool value)
{
	glUniform1i(location, (int)value);
}

void 
fe_shader_set_int(fe_shader_get_location_id location, int value)
{
	glUniform1i(location, value);
}

void 
fe_shader_set_f32(fe_shader_get_location_id location, float value)
{
	glUniform1f(location, value);
}

void 
fe_shader_set_vec2(fe_shader_get_location_id location, const fe_vec2_t *value)
{
	glUniform2fv(location, 1, value->raw);
}

void 
fe_shader_set_vec3(fe_shader_get_location_id location, const fe_vec3_t *value)
{
	glUniform3fv(location, 1, value->raw);
}

void 
fe_shader_set_vec4(fe_shader_get_location_id location, const fe_vec4_t *value)
{
	glUniform4fv(location, 1, value->raw);
}

void 
fe_shader_set_mat4x4(fe_shader_get_location_id location, const fe_mat4x4_t *value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, value->raw[0]);
}


