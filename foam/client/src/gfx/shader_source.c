#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <foam/client/gfx/gl.h>
#include <foam/client/gfx/shader.h>
#include <foam/client/gfx/shader_source.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/logger.h>
#include <foam/core/ext/string.h>
#include <foam/core/ext/io.h>

#define FE_VS_TEXT_START     "@FE_VERT"
#define FE_VS_TEXT_END       "FE_VERT@"
#define FE_FS_TEXT_START     "@FE_FRAG"
#define FE_FS_TEXT_END       "FE_FRAG@"

#define FE_TEXT_HEADER_LEN 8

const char *GL_VER[] =
{
	"#version 330 core\n",
	"#version 300 es\nprecision mediump float;\nprecision mediump sampler2DArray;\n"
};

const char*
get_gl_version(void)
{     
	u32 gl_index = 0;
#ifdef FE_OPENGL_ES
	gl_index = 1;
#endif
    return GL_VER[gl_index];
}

FE_RETURN_CODE
fe_shader_source_make(fe_shader_source_t *src)
{
    FE_RETURN_CODE rc = FE_OK;

    const char *ver = get_gl_version();

    rc = fe_writer_make(&src->vertex_data);
    rc = fe_writer_make(&src->fragment_data);

    fe_io_write_char(&src->vertex_data, ver, strlen(ver));
    fe_io_write_char(&src->fragment_data, ver, strlen(ver));

    src->path = NULL;

    return rc;
}

FE_RETURN_CODE 
fe_shader_source_load_buffers(fe_shader_source_t *src, 
                             const char *vertex_buffer, size_t v_len, 
                             const char *fragment_buffer, size_t f_len)
{
    FE_RETURN_CODE rc = FE_OK;

    fe_io_write_char(&src->vertex_data, vertex_buffer, v_len);
    fe_io_write_null_terminator(&src->vertex_data);
    fe_io_write_char(&src->fragment_data, fragment_buffer, f_len);
    fe_io_write_null_terminator(&src->fragment_data);

    return rc;
}

FE_RETURN_CODE
fe_shader_source_make_from_file(fe_shader_source_t *src, const char *path)
{
    FE_RETURN_CODE rc = FE_OK;

    fe_shader_source_make(src);
    
    fe_reader_t reader;
    rc = fe_reader_make_from_file(&reader, path);

    if (rc != FE_OK) 
    {
        fe_log_return_code(rc, "Shader source '%s' could not be initialized", path);
        return rc;
    }

    char *vs_start = strstr((char*)reader.buffer, FE_VS_TEXT_START);
    char *vs_end   = strstr((char*)reader.buffer, FE_VS_TEXT_END);

    char *fs_start = strstr((char*)reader.buffer, FE_FS_TEXT_START);
    char *fs_end   = strstr((char*)reader.buffer, FE_FS_TEXT_END);

    if (!vs_start || !vs_end || !fs_start || !fs_end)
    {
        fe_log("Shader source '%s' is missing headers!");
        fe_reader_kill(&reader);
        return FE_ERR;
    }

    ptrdiff_t vs_diff = (vs_end - 1) - (vs_start + FE_TEXT_HEADER_LEN);
    ptrdiff_t fs_diff = (fs_end - 1) - (fs_start + FE_TEXT_HEADER_LEN);

    char *vs_buffer = malloc(vs_diff);
    memcpy(vs_buffer, (vs_start + FE_TEXT_HEADER_LEN), vs_diff);

    char *fs_buffer = malloc(fs_diff);
    memcpy(fs_buffer, (fs_start + FE_TEXT_HEADER_LEN), fs_diff);

    fe_shader_source_load_buffers(src, vs_buffer, vs_diff, fs_buffer, fs_diff);

    free(vs_buffer);
    free(fs_buffer);
    fe_reader_kill(&reader);

    src->path = strdup(path);

    return rc;    
}

void         
fe_shader_source_kill(fe_shader_source_t *src)
{
	fe_writer_kill(&src->vertex_data);
	fe_writer_kill(&src->fragment_data);
    free(src->path);
}

void
fe_shader_source_add_define(fe_shader_source_t *src, FE_SHADER_SOURCE_TYPE type, const char *define)
{
    fe_writer_t *writer = (type == FE_SHADER_SOURCE_VERTEX) ? &src->vertex_data : &src->fragment_data;

    const char *d = "#define ";
    
    fe_io_write_data(writer, d, strlen(d));
    fe_io_write_data(writer, define, strlen(define));
    fe_io_write_null_terminator(writer);
}

void
fe_shader_source_add_define_f32(fe_shader_source_t *src, FE_SHADER_SOURCE_TYPE type, const char *define, f32 val)
{
    const size_t def_len = strlen(define) + 64;
    char def[def_len];
    memset(def, '\0', def_len);
    sprintf(def, "%s %f", define, val);

    fe_shader_source_add_define(src, type, def);
}

void
fe_shader_source_add_define_i32(fe_shader_source_t *src, FE_SHADER_SOURCE_TYPE type, const char *define, i32 val)
{
    const size_t def_len = strlen(define) + 64;
    char def[def_len];
    memset(def, '\0', def_len);
    sprintf(def, "%s %d", define, val);

    fe_shader_source_add_define(src, type, def);
}

void
fe_shader_source_print(fe_shader_source_t *src)
{
    fe_log("\n-- VERTEX --\n%s", src->vertex_data.buffer);
    fe_log("\n-- FRAGMENT --\n%s", src->fragment_data.buffer);
}

