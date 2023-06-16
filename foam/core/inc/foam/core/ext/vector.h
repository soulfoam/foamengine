#pragma once

#include <foam/core/ext/sugar.h>
#include <assert.h>

#define FE_INVALID_INDEX ((u64)-1)

typedef struct
{
    u64 capacity;
    u64 count;
} fe_vec_header_t;

typedef void (*fe_vec_free_fn)();
static inline void FE_VEC_CALL_FREE_PTR(fe_vec_free_fn f, void *del) { if (f) f(del); }

#define FE_VEC_EMPTY NULL
#define fe_vec(type) type*

#define FE_VEC_HEADER_SIZE    (sizeof(fe_vec_header_t))
#define FE_VEC_HEADER_PTR(v) (v) ? ((void*)v-FE_VEC_HEADER_SIZE) : NULL

static inline u64 
fe_vec_get_capacity(void *v)
{
    if (!v) return 0;
    fe_vec_header_t* hdr = FE_VEC_HEADER_PTR(v); 
    return hdr->capacity;
}

static inline u64 
fe_vec_get_count(void *v)
{
    if (!v) return 0;
    fe_vec_header_t* hdr = FE_VEC_HEADER_PTR(v); 
    return hdr->count;
}

static inline void 
FE_VEC_SET_CAPACITY(void *v, u64 cap)
{
    if (!v) return;
    fe_vec_header_t* hdr = FE_VEC_HEADER_PTR(v); 
    hdr->capacity = cap;
}

static inline void 
FE_VEC_SET_COUNT(void *v, u64 count)
{
    if (!v) return;
    fe_vec_header_t* hdr = FE_VEC_HEADER_PTR(v); 
    hdr->count = count;
}

#define fe_vec_make(v, cap)                                                                        \
do                                                                                                  \
{                                                                                                   \
    v = FE_VEC_EMPTY;                                                                              \
    _fe_vec_grow(v, cap);                                                                         \
    FE_VEC_SET_COUNT(v, 0);                                                                        \
} while(0)

#define fe_vec_make_with_count(v, cap)                                                             \
do                                                                                                  \
{                                                                                                   \
    v = FE_VEC_EMPTY;                                                                              \
    _fe_vec_grow(v, cap);                                                                         \
    FE_VEC_SET_COUNT(v, cap);                                                                      \
} while(0)

#define fe_vec_kill(v, free_fn)                                                                    \
do                                                                                                  \
{                                                                                                   \
    for (u64 _i = 0; _i < fe_vec_get_count(v); _i++) FE_VEC_CALL_FREE_PTR(free_fn, &v[_i]);             \
    free(FE_VEC_HEADER_PTR(v));                                                                    \
    v = FE_VEC_EMPTY;                                                                              \
} while(0)
#define fe_vec_clear(v, free_fn)                                                                    \
do                                                                                                  \
{                                                                                                   \
    for (u64 _i = 0; _i < fe_vec_get_count(v); _i++) FE_VEC_CALL_FREE_PTR(free_fn, &v[_i]);             \
    FE_VEC_SET_COUNT(v, 0); \
} while(0)

#define fe_vec_add(v, val)                                                                         \
do                                                                                                  \
{                                                                                                   \
    _fe_vec_grow_if_needed(v);                                                                    \
    v[_count] = val;                                                                              \
} while(0)

#define fe_vec_add_and_get(v, ptr)                                                                 \
do                                                                                                  \
{                                                                                                   \
    _fe_vec_grow_if_needed(v);                                                                    \
    ptr = &v[_count];                                                                             \
} while(0)

#define fe_vec_append(dst, src) for (u64 i = 0; i < fe_vec_get_count(src); i++) fe_vec_add(dst, src[i])

#define fe_vec_get(v, i) &v[i]
#define fe_vec_del(v, i, free_fn)                                                                  \
do                                                                                                  \
{                                                                                                   \
    u64 _count = fe_vec_get_count(v);                                                                 \
    FE_VEC_CALL_FREE_PTR(free_fn, &v[i]);                                                          \
    v[i] = v[_count-1];                                                                          \
    FE_VEC_SET_COUNT(v, fe_vec_get_count(v)-1);                                                       \
} while(0)

#define fe_vec_pop(v, free_fn) fe_vec_del(v, fe_vec_get_last_index(v), free_fn);

#define fe_vec_contains(v, item, dst)                                                                  \
do                                                                                                  \
{ \
    dst = FE_INVALID_INDEX; \
    for (u64 _i = 0; _i < fe_vec_get_count(v); _i++) if (v[_i] == item) {dst = _i; break;} \
} while(0)

#define fe_vec_for_each(l, itm)                                                                     \
for (u64 _fe_for_each_index = 0; (_fe_for_each_index == 0); ++_fe_for_each_index)             \
for (__typeof__(*l) *itm = l; _fe_for_each_index < fe_vec_get_count(l); itm=&l[++_fe_for_each_index])

#define fe_vec_for_each_index(l, itm, i)                                                                     \
for (u64 i = 0; (i == 0); ++i)             \
for (__typeof__(*l) *itm = l; i < fe_vec_get_count(l); itm=&l[++i])

#define fe_vec_is_empty(v)   (fe_vec_get_count(v) == 0)
#define fe_vec_get_capacity(v)   fe_vec_get_capacity(v)
#define fe_vec_get_last_index(v) fe_vec_get_count(v)-1

#define _fe_vec_grow(v, cap)                                                                      \
do                                                                                                  \
{                                                                                                   \
    void *_d           = FE_VEC_HEADER_PTR(v);                                                    \
    void *_data        = realloc(_d, FE_VEC_HEADER_SIZE + (sizeof(*v) * cap));                   \
    assert(_data);                                                                                 \
    fe_vec_header_t *_hdr = _data;                                                                   \
    _hdr->capacity     = cap;                                                                      \
    (v)                = (void*)(_data + FE_VEC_HEADER_SIZE);                                     \
} while (0)

#define _fe_vec_grow_if_needed(v)                                                                 \
    u64 _count = fe_vec_get_count(v);                                                                 \
    u64 _cap   = fe_vec_get_capacity(v);                                                              \
    if (_cap <= _count)                                                                           \
    {                                                                                               \
        if (_cap == 0) _cap = 1;                                                                  \
        _fe_vec_grow(v, _cap * 2);                                                               \
    }                                                                                               \
    FE_VEC_SET_COUNT(v, _count+1);                                                                \

