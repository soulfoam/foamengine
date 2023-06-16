#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/logger.h>
#include <foam/core/return_code.h>

typedef struct
{
    const u8 *buffer;
    size_t size;
    size_t offset;
} fe_reader_t;

typedef struct
{
    u8 *buffer;
    size_t capacity;
    size_t offset;
    size_t size;
    bool dynamic;
} fe_writer_t;

static inline u32 
htonf(f32 f)
{
    u32 p;
    u32 sign;

    if (f < 0) { sign = 1; f = -f; }
    else { sign = 0; }
        
    p = ((((u32)f)&0x7fff)<<16) | (sign<<31); // whole part and sign
    p |= (u32)(((f - (i32)f) * 65536.0f))&0xffff; // fraction

    return p;
}

static inline f32 
ntohf(u32 p)
{
    f32 f = ((p>>16)&0x7fff); // whole part
    f += (p&0xffff) / 65536.0f; // fraction

    if (((p>>31)&0x1) == 0x1) { f = -f; } // sign bit set

    return f;
}

static inline FE_RETURN_CODE
fe_writer_init_buffer(fe_writer_t *writer, 
                         u8 *buffer, size_t buf_size, size_t buf_capacity, 
                         size_t offset, 
                         bool dynamic)
{
    writer->buffer   = buffer;
    writer->size     = buf_size;
    writer->capacity = buf_capacity;
    writer->offset   = offset;
    writer->dynamic  = dynamic;

    return FE_OK;
}


static inline FE_RETURN_CODE
fe_writer_make(fe_writer_t *writer)
{
    size_t buf_capacity = 4096;

    fe_writer_init_buffer(writer, malloc(buf_capacity), 0, buf_capacity, 0, true);


    return FE_OK;
}

static inline FE_RETURN_CODE
fe_writer_make_capacity(fe_writer_t *writer, size_t starting_capacity, bool dynamic)
{
    fe_writer_init_buffer(writer, malloc(starting_capacity), 0, starting_capacity, 0, dynamic);

    return FE_OK;
}

static inline bool
fe_writer_is_empty(const fe_writer_t *writer)
{
    return writer->size == 0;
}

static inline void
fe_writer_kill(fe_writer_t *writer)
{
    free(writer->buffer);
}

static inline size_t
fe_writer_get_capacity(const fe_writer_t *writer)
{
    return writer->capacity;
}

static inline size_t
fe_writer_get_size(const fe_writer_t *writer)
{
    return writer->size;
}

static inline u8*
fe_writer_get_buffer(const fe_writer_t *writer)
{
    return writer->buffer;
}

static inline void
fe_writer_clear(fe_writer_t *writer)
{
    writer->size = 0;
    writer->offset = 0;
}

static inline bool
fe_writer_save(fe_writer_t *writer, const char *path, const char *file_opt)
{
	FILE *fp = fopen(path, file_opt);
	if (fp == NULL)
	{
		fe_log_warn("fe_writer_save failed for file %s", path);
		return false;
	}

	fwrite(writer->buffer, sizeof(u8), writer->size, fp);

	fclose(fp);

    return true;
}

static inline bool
fe_writer_save_append(fe_writer_t *writer, const char *path)
{
	FILE *fp = fopen(path, "wb");
	if (fp == NULL)
	{
		fe_log_warn("fe_writer_save_append failed for file %s", path);
		return false;
	}

	fwrite(writer->buffer, sizeof(u8), writer->size, fp);

	fclose(fp);

    return true;
}

static inline FE_RETURN_CODE
fe_reader_make_from_file(fe_reader_t *reader, const char *path)
{
	FILE *fp = fopen(path, "rb");
	if (fp == NULL)
	{
		fe_log_return_code(FE_ERR_PATH_NOT_FOUND, "fe_reader_t failed to open file: %s", path);
		return FE_ERR_PATH_NOT_FOUND;
	}

	fseek(fp, 0L, SEEK_END);
	size_t file_size = 0;
	file_size = ftell(fp);
    rewind(fp);

	reader->buffer = malloc(file_size);
    reader->size   = file_size;
    reader->offset = 0;
	fread((char*)reader->buffer, sizeof(u8), file_size, fp);

	fclose(fp);

    return FE_OK;
}

static inline void
fe_reader_make_from_buffer(fe_reader_t *reader, const u8 *data, size_t data_len)
{
	reader->buffer = malloc(data_len);
    reader->size   = data_len;
    memcpy((char*)reader->buffer, data, data_len);
    reader->offset = 0;
}

static inline void
fe_reader_init_buffer(fe_reader_t *reader, const u8 *data, size_t data_len)
{
	reader->buffer = data;
    reader->size   = data_len;
    reader->offset = 0;
}

static inline void
fe_reader_kill(fe_reader_t *reader)
{
    free((char*)reader->buffer);
}

static inline size_t
fe_reader_get_size(const fe_reader_t *reader)
{
    return reader->size;
}

static inline const u8*
fe_reader_get_buffer(const fe_reader_t *reader)
{
    return reader->buffer;
}

static inline void 
fe_reader_set_offset(fe_reader_t *reader, size_t offset)
{
    reader->offset = offset;
}

static inline bool
fe_io_write_data(fe_writer_t *writer, const void *data, size_t data_len)
{
    if (writer->capacity < (data_len + writer->size))
    {
        if (writer->dynamic)
        {
            writer->buffer = realloc(writer->buffer, (writer->capacity *= 2) * sizeof(*writer->buffer));
        }
        else
        {
            fe_log_warn("Stopped writer from overflowing! Attempted to write %lu bytes into a buffer with only %lu bytes left", data_len, writer->capacity-writer->size);
            return false;
        }
    }

    memcpy(writer->buffer + writer->offset, data, data_len);
    writer->size += data_len;
    writer->offset += data_len;

    return true;
}

static inline void
fe_io_read_data(fe_reader_t *reader, void *data, size_t data_len)
{
    memcpy(data, reader->buffer + reader->offset, data_len);
    reader->offset += data_len;
}

static inline u32 
fe_io_write_char(fe_writer_t *writer, const char *data, size_t len)
{
    fe_io_write_data(writer, data, len);
	return len;
}

static inline u32 
fe_io_write_null_terminator(fe_writer_t *writer)
{
    fe_io_write_data(writer, "\0", 1);
	return 1;
}

static inline u32 
fe_io_read_char(fe_reader_t *reader, char *data, size_t len)
{
    fe_io_read_data(reader, data, len);
	return len;
}

static inline void
fe_io_write_u8(fe_writer_t *writer, u8 data)
{
    fe_io_write_data(writer, &data, sizeof(u8));
}

static inline u8 
fe_io_read_u8(fe_reader_t *reader)
{
    u8 ret = 0;
    fe_io_read_data(reader, &ret, sizeof(u8));

    return ret;
}

static inline void
fe_io_write_i8(fe_writer_t *writer, i8 data)
{
    fe_io_write_data(writer, &data, sizeof(i8));
}

static inline i8 
fe_io_read_i8(fe_reader_t *reader)
{
    i8 ret = 0;
    fe_io_read_data(reader, &ret, sizeof(i8));

    return ret;
}

static inline void 
fe_io_write_u16(fe_writer_t *writer, u16 data)
{
    u8 bytes[sizeof(u16)];
    bytes[0] = (data >> 8) & 0xFF;
    bytes[1] = (data >> 0) & 0xFF;
    fe_io_write_data(writer, bytes, sizeof(u16));
}

static inline u16 
fe_io_read_u16(fe_reader_t *reader)
{
    u8 bytes[sizeof(u16)];
    fe_io_read_data(reader, bytes, sizeof(u16));

    return 
    ( 
        ((u16)(bytes[1] << 0)) | 
        ((u16)(bytes[0] << 8))
    );
}

static inline void 
fe_io_write_i16(fe_writer_t *writer, i16 data)
{
    fe_io_write_u16(writer, (u16)data);
}

static inline i16 
fe_io_read_i16(fe_reader_t *reader)
{
    return (i16)fe_io_read_u16(reader);
}

static inline void 
fe_io_write_u32(fe_writer_t *writer, u32 data)
{
    u8 bytes[sizeof(u32)];
    bytes[0] = (data >> 24) & 0xFF;
    bytes[1] = (data >> 16) & 0xFF;
    bytes[2] = (data >>  8) & 0xFF;
    bytes[3] = (data >>  0) & 0xFF;
    fe_io_write_data(writer, bytes, sizeof(u32));
}

static inline u32 
fe_io_read_u32(fe_reader_t *reader)
{
    u8 bytes[sizeof(u32)];
    fe_io_read_data(reader, bytes, sizeof(u32));

    return 
    (
        ((u32)bytes[3] << 0)  | 
        ((u32)bytes[2] << 8)  | 
        ((u32)bytes[1] << 16) | 
        ((u32)bytes[0] << 24) 
    );
}

static inline void 
fe_io_write_u64(fe_writer_t *writer, u64 data)
{
    u8 bytes[sizeof(u64)];
    bytes[0] = (data >> 56) & 0xFF;
    bytes[1] = (data >> 48) & 0xFF;
    bytes[2] = (data >> 40) & 0xFF;
    bytes[3] = (data >> 32) & 0xFF;
    bytes[4] = (data >> 24) & 0xFF;
    bytes[5] = (data >> 16) & 0xFF;
    bytes[6] = (data >>  8) & 0xFF;
    bytes[7] = (data >>  0) & 0xFF;
    fe_io_write_data(writer, bytes, sizeof(u64));
}

static inline u64
fe_io_read_u64(fe_reader_t *reader)
{
    u8 bytes[sizeof(u64)];
    fe_io_read_data(reader, bytes, sizeof(u64));

    return 
    (
        ((u64)bytes[7] << 0)  |
        ((u64)bytes[6] << 8)  |
        ((u64)bytes[5] << 16) |
        ((u64)bytes[4] << 24) |
        ((u64)bytes[3] << 32) |
        ((u64)bytes[2] << 40) |
        ((u64)bytes[1] << 48) |
        ((u64)bytes[0] << 56) 
    );
}

static inline void 
fe_io_write_i32(fe_writer_t *writer, i32 data)
{
    fe_io_write_u32(writer, (u32)data);
}

static inline i32 
fe_io_read_i32(fe_reader_t *reader)
{
    return (i32)fe_io_read_u32(reader);
}

static inline void 
fe_io_write_f32(fe_writer_t *writer, f32 data)
{
    fe_io_write_u32(writer, htonf(data));
}

static inline f32 
fe_io_read_f32(fe_reader_t *reader)
{
    return ntohf(fe_io_read_u32(reader));
}

static inline u32 
fe_io_write_string(fe_writer_t *writer, const char *data)
{
    u32 len = strlen(data);
    fe_io_write_u32(writer, len);
    fe_io_write_data(writer, data, len);
	return len;
}

static inline char* 
fe_io_read_string(fe_reader_t *reader)
{
    u32 len = fe_io_read_u32(reader);
    char *string = malloc(len+1);
	fe_io_read_data(reader, string, len);
    string[len] = '\0';

    return string;
}

static inline void 
fe_io_write_vec2(fe_writer_t *writer, const fe_vec2_t *v)
{
	for (u8 i = 0; i < 2; i++)
	{
		fe_io_write_f32(writer, v->raw[i]);
	}
}

static inline fe_vec2_t 
fe_io_read_vec2(fe_reader_t *reader)
{
    fe_vec2_t v;
	for (u8 i = 0; i < 2; i++)
	{
		v.raw[i] = fe_io_read_f32(reader);
	}
    return v;
}

static inline void 
fe_io_write_vec3(fe_writer_t *writer, const fe_vec3_t *v)
{
	for (u8 i = 0; i < 3; i++)
	{
		fe_io_write_f32(writer, v->raw[i]);
	}
}

static inline fe_vec3_t 
fe_io_read_vec3(fe_reader_t *reader)
{
    fe_vec3_t v;
	for (u8 i = 0; i < 3; i++)
	{
		v.raw[i] = fe_io_read_f32(reader);
	}

    return v;
}

static inline void 
fe_io_write_vec4(fe_writer_t *writer, const fe_vec4_t *v)
{
	for (u8 i = 0; i < 4; i++)
	{
		fe_io_write_f32(writer, v->raw[i]);
	}
}

static inline fe_vec4_t 
fe_io_read_vec4(fe_reader_t *reader)
{
    fe_vec4_t v;
	for (u8 i = 0; i < 4; i++)
	{
		v.raw[i] = fe_io_read_f32(reader);
	}

    return v;
}

static inline void 
fe_io_write_quat(fe_writer_t *writer, const fe_quat_t *v)
{
	for (u8 i = 0; i < 4; i++)
	{
		fe_io_write_f32(writer, v->raw[i]);
	}
}

static inline fe_quat_t 
fe_io_read_quat(fe_reader_t *reader)
{
    fe_quat_t v;
	for (u8 i = 0; i < 4; i++)
	{
		v.raw[i] = fe_io_read_f32(reader);
	}

    return v;
}

static inline void 
fe_io_write_mat4x4(fe_writer_t *writer, const fe_mat4x4_t *m)
{
	for (u8 i = 0; i < 4; i++)
	{
        for (u8 j = 0; j < 4; j++)
        {
            fe_io_write_f32(writer, m->raw[i][j]);
        }
	} 
}

static inline fe_mat4x4_t 
fe_io_read_mat4x4(fe_reader_t *reader)
{
    fe_mat4x4_t m;
	for (u8 i = 0; i < 4; i++)
	{
        for (u8 j = 0; j < 4; j++)
        {
            m.raw[i][j] = fe_io_read_f32(reader);
        }
	} 

    return m;
}

