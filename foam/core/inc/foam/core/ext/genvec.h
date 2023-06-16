#pragma once

#include <foam/core/ext/sugar.h>
#include <foam/core/ext/logger.h>
#include <assert.h>

typedef struct
{
    u32 version;
    bool in_use;
} fe_genvec_data_t;

typedef struct
{
    u32 capacity;
    u32 count;
    u32 max_slot_num;
    fe_genvec_data_t *data;
} fe_genvec_header_t;

typedef void (*fe_genvec_free_fn)();
static inline void FE_GENVEC_CALL_FREE_PTR(fe_genvec_free_fn f, void *del) { if (f) f(del); }

typedef u64 fe_genvec_id;
#define FE_GENVEC_EMPTY NULL
#define fe_genvec(type) type*

#define FE_GENVEC_HEADER_SIZE    (sizeof(fe_genvec_header_t))
#define FE_GENVEC_HEADER_PTR(gv) (gv) ? ((void*)gv-FE_GENVEC_HEADER_SIZE) : NULL

#define FE_GENVEC_ID(index, ver) (((u64)ver << 32) | index)
#define FE_GENVEC_INDEX(id)      ((u32)id)
#define FE_GENVEC_VERSION(id)    ((u32)((u64)id >> 32))

static inline void
FE_GENVEC_HEADER_MAKE(fe_genvec_header_t *hdr)
{
    hdr->count        = 0;
    hdr->max_slot_num = 0;
    hdr->data         = NULL;
}

static inline void
FE_GENVEC_CALC_MAX_SLOT_NUM(fe_genvec_header_t *hdr)
{
    for (u32 i = hdr->capacity; i --> 0;)
    {
        if (hdr->data[i].in_use) 
        {
            hdr->max_slot_num = i;
            return;
        }
    }

    hdr->max_slot_num = 0;
}

static inline fe_genvec_id
FE_GENVEC_DATA_GRAB_BLOCK(fe_genvec_header_t *hdr)
{
    fe_genvec_id res = 0;

    for (u32 i = 0; i < hdr->capacity; i++)
    {
        fe_genvec_data_t *d = &hdr->data[i];
        if (!d->in_use)
        {
            d->in_use = true;
            res = FE_GENVEC_ID(i, d->version);
            if (res == FE_INVALID_ID) res = FE_GENVEC_ID(i, ++d->version);
            if (i > hdr->max_slot_num) hdr->max_slot_num = i;

            break;
        }
    }

    hdr->count++;

    return res;
}

static inline bool
FE_GENVEC_DATA_PUT_BLOCK(fe_genvec_header_t *hdr, fe_genvec_id id)
{
    u32 index = FE_GENVEC_INDEX(id);
    if (index >= hdr->capacity) return false;

    fe_genvec_data_t *d = &hdr->data[index];
    d->version += 1;
    d->in_use   = false;

    FE_GENVEC_CALC_MAX_SLOT_NUM(hdr);

    if (hdr->count > 0) hdr->count--;

    return true;
}

static inline void
FE_GENVEC_DATA_GROW(fe_genvec_header_t *hdr, u32 old_capacity)
{
    fe_genvec_data_t *tmp = realloc(hdr->data, sizeof(*hdr->data) * hdr->capacity);
    if (tmp) hdr->data = tmp;
    for (u32 i = hdr->capacity; i --> old_capacity;)
    {
        hdr->data[i].version = 0;
        hdr->data[i].in_use  = false;
    }
}

static inline u32 
FE_GENVEC_MAX_SLOT_NUM(const void *gv)
{
    if (!gv) return 0;
    fe_genvec_header_t* hdr = FE_GENVEC_HEADER_PTR(gv); 
    return hdr->max_slot_num;
}

static inline u32 
fe_genvec_get_capacity(const void *gv)
{
    if (!gv) return 0;
    fe_genvec_header_t* hdr = FE_GENVEC_HEADER_PTR(gv); 
    return hdr->capacity;
}

static inline u32 
fe_genvec_get_count(const void *gv)
{
    if (!gv) return 0;
    fe_genvec_header_t* hdr = FE_GENVEC_HEADER_PTR(gv); 
    return hdr->count;
}

static inline void 
FE_GENVEC_SET_CAPACITY(const void *gv, u32 cap)
{
    if (!gv) return;
    fe_genvec_header_t* hdr = FE_GENVEC_HEADER_PTR(gv); 
    hdr->capacity = cap;
}

static inline void 
FE_GENVEC_SET_COUNT(const void *gv, u32 count)
{
    if (!gv) return;
    fe_genvec_header_t* hdr = FE_GENVEC_HEADER_PTR(gv); 
    hdr->count = count;
}

static inline bool
fe_genvec_is_valid_id(const void *gv, fe_genvec_id id)
{
    if (!gv || id == FE_INVALID_ID) return false;
    fe_genvec_header_t* hdr = FE_GENVEC_HEADER_PTR(gv); 
    u32 index            = FE_GENVEC_INDEX(id); 
    u32 version          = FE_GENVEC_VERSION(id); 
    if (!hdr || index >= hdr->capacity || version != hdr->data[index].version) return false;

    return true;
}

static inline bool
fe_genvec_valid_index(const void *gv, u32 index)
{
    if (!gv) return false;
    fe_genvec_header_t* hdr = FE_GENVEC_HEADER_PTR(gv); 
    if (!hdr || index >= hdr->capacity) return false;
    if (!hdr->data[index].in_use)       return false;

    return true;
}

static inline fe_genvec_id
fe_genvec_id_at(const void *gv, u32 index)
{
    if (!gv) return FE_INVALID_ID;
    fe_genvec_header_t* hdr = FE_GENVEC_HEADER_PTR(gv); 

    return FE_GENVEC_ID(index, hdr->data[index].version);
}

static inline void*
FE_GENVEC_FOR_EACH_FIND_PTR(void *gv, u32 *index, size_t gv_size)
{
    if (!gv) return NULL;

    fe_genvec_header_t* hdr = FE_GENVEC_HEADER_PTR(gv); 

    if (*index > hdr->max_slot_num || hdr->max_slot_num == 0) return NULL;

    while (!hdr->data[*index].in_use)
    {
        *index += 1;
    }

    void *r = gv + (*index * gv_size);
    *index += 1;

    return r;

}

static inline u32
FE_GENVEC_FOR_EACH_FIND_INDEX(void *gv, u32 *index)
{
    fe_genvec_header_t* hdr = FE_GENVEC_HEADER_PTR(gv); 
    if (*index+1 == hdr->capacity) return *index;

    *index += 1;

    while (!hdr->data[*index].in_use)
    {
        if (*index+1 == hdr->capacity) break;
        *index += 1; 
    }

    return *index;
}

#define fe_genvec_make(gv, cap) _fe_genvec_grow(gv, cap)
#define fe_genvec_kill(gv, free_fn)                                                          \
do                                                                                           \
{                                                                                            \
    fe_genvec_header_t *_hdr = FE_GENVEC_HEADER_PTR(gv);                                        \
    if (_hdr)                                                                                \
    {                                                                                        \
        for (u32 _i = 0; _i < _hdr->capacity; _i++)                                          \
        if (_hdr->data[_i].in_use) FE_GENVEC_CALL_FREE_PTR(free_fn, &gv[_i]);                \
        free(_hdr->data);                                                                    \
        free(_hdr);                                                                          \
        gv = FE_GENVEC_EMPTY;                                                                \
    }                                                                                        \
} while(0)

#define fe_genvec_add(gv, dstid, val)                                                        \
do                                                                                           \
{                                                                                            \
    _fe_genvec_grow_if_needed(gv);                                                           \
    dstid = FE_GENVEC_DATA_GRAB_BLOCK(FE_GENVEC_HEADER_PTR(gv));                             \
    gv[FE_GENVEC_INDEX(dstid)] = val;                                                        \
} while(0)

#define fe_genvec_add_and_get(gv, dstid, ptr)                                                \
do                                                                                           \
{                                                                                            \
    _fe_genvec_grow_if_needed(gv);                                                           \
    dstid = FE_GENVEC_DATA_GRAB_BLOCK(FE_GENVEC_HEADER_PTR(gv));                             \
    ptr = &gv[FE_GENVEC_INDEX(dstid)];                                                       \
} while(0)

#define fe_genvec_get(gv, id) (fe_genvec_is_valid_id(gv, id) ? &gv[FE_GENVEC_INDEX(id)] : NULL)
#define fe_genvec_del(gv, id, free_fn)                                                       \
if (gv && FE_GENVEC_DATA_PUT_BLOCK(FE_GENVEC_HEADER_PTR(gv), id))                                  \
{FE_GENVEC_CALL_FREE_PTR(free_fn, &gv[FE_GENVEC_INDEX(id)]);}                                \

#define fe_genvec_for_each(l, itm)                                                           \
for (u32 _fe_for_each_index=0; (_fe_for_each_index == 0); ++_fe_for_each_index)                                                              \
for (__typeof__(l) itm = FE_GENVEC_FOR_EACH_FIND_PTR(l, &_fe_for_each_index, sizeof(__typeof__(*l)));                                                                \
                    itm != NULL;                                                            \
                    itm = FE_GENVEC_FOR_EACH_FIND_PTR(l, &_fe_for_each_index, sizeof(__typeof__(*l))))

#define fe_genvec_for_each_index(l, itm, ind)                                                           \
for (u32 ind=0; (ind == 0); ++ind)                                                              \
for (__typeof__(l) itm = FE_GENVEC_FOR_EACH_FIND_PTR(l, &ind, sizeof(__typeof__(*l)));                                                                \
                    itm != NULL;                                                            \
                    itm = FE_GENVEC_FOR_EACH_FIND_PTR(l, &ind, sizeof(__typeof__(*l))))
#define fe_genvec_for_each_cur_id(gv)    fe_genvec_id_at(gv, _fe_for_each_index-1)


#define _fe_genvec_grow(gv, cap)                                                             \
do                                                                                           \
{                                                                                            \
    void *_d        = FE_GENVEC_HEADER_PTR(gv);                                              \
    u32 _cur_cap    = fe_genvec_get_capacity(gv);                                                \
    void *_data     = realloc(_d, FE_GENVEC_HEADER_SIZE + (sizeof(*gv) * cap));              \
    assert(_data);                                                                           \
    fe_genvec_header_t *_hdr = _data;                                                           \
    _hdr->capacity = cap;                                                                    \
    if (!_d) FE_GENVEC_HEADER_MAKE(_hdr);                                                    \
    FE_GENVEC_DATA_GROW(_hdr, _cur_cap);                                                     \
    (gv) = (void*)(_data + FE_GENVEC_HEADER_SIZE);                                           \
} while (0)

#define _fe_genvec_grow_if_needed(gv)                                                        \
{                                                                                            \
    u32 _count = fe_genvec_get_count(gv);                                                        \
    u32 _cap   = fe_genvec_get_capacity(gv);                                                     \
    if (_cap <= _count)                                                                      \
    {                                                                                        \
        if (_cap == 0) _cap = 1;                                                             \
        _fe_genvec_grow(gv, _cap * 2);                                                       \
    }                                                                                        \
}

