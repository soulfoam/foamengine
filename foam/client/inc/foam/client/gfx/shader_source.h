#pragma once

#include <string.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/io.h>
#include <foam/core/return_code.h>

typedef enum
{
    FE_SHADER_SOURCE_VERTEX,
    FE_SHADER_SOURCE_FRAGMENT

} FE_SHADER_SOURCE_TYPE;

typedef struct
{
    char *path;
    fe_writer_t vertex_data;
    fe_writer_t fragment_data;

} fe_shader_source_t;

FE_RETURN_CODE fe_shader_source_make(fe_shader_source_t *src);
FE_RETURN_CODE fe_shader_source_make_from_file(fe_shader_source_t *src, const char *path);
FE_RETURN_CODE fe_shader_source_load_buffers(fe_shader_source_t *src, const char *vertex_buffer, size_t v_len, const char *fragment_buffer, size_t f_len);
void           fe_shader_source_kill(fe_shader_source_t *src);
void           fe_shader_source_add_define(fe_shader_source_t *src, FE_SHADER_SOURCE_TYPE type, const char *define);
void           fe_shader_source_add_define_f32(fe_shader_source_t *src, FE_SHADER_SOURCE_TYPE type, const char *define, f32 val);
void           fe_shader_source_add_define_i32(fe_shader_source_t *src, FE_SHADER_SOURCE_TYPE type, const char *define, i32 val);
void           fe_shader_source_print(fe_shader_source_t *src);
