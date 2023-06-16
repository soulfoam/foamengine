#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t	 u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t	 i32;
typedef int64_t	 i64;
typedef float    f32;
typedef double   f64;

#define ARRAY_SIZE(array)      (sizeof(array)/sizeof(array[0]))

static inline bool STR_EQ(const char *str0, const char *str1)
{
    if (!str0 && !str1) return true;
    if (!str0 || !str1) return false;
    return strcmp(str0, str1) == 0;
}

static inline bool STR_NOT_EQ(const char *str0, const char *str1)
{
    if (!str0 && !str1) return false;
    if (!str0 || !str1) return true;
    return strcmp(str0, str1) != 0;
}

#define FE_DEFINE_HANDLE(name)  \
typedef struct { u64 id; } name \

#define FE_VALID_HANDLE_ID(h) (h.id != FE_INVALID_ID)
#define FE_INVALID_HANDLE_ID(h) (h.id == FE_INVALID_ID)
#define FE_INVALID_ID   ((u64)-1)
#define FE_HANDLE_EQ(h, ho) (h.id == ho.id)

static inline u64
fe_random_u64(void)
{
    u64 r = 0;
    for (int i=0; i<64; i += 15 /*30*/)
    {
        r = r*((u64)RAND_MAX + 1) + rand();
    }
    return r;
}

