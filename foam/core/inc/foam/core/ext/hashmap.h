#pragma once

#include <foam/core/ext/vector.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/return_code.h>

typedef struct
fe_hashmap_item_t
{
    char *key;
    u64 val;
} fe_hashmap_item_t;

typedef struct
fe_hashmap_t
{
    fe_vec(fe_hashmap_item_t) items;

    u64 item_count;
    u64 capacity;
} fe_hashmap_t;

void         fe_hashmap_make(fe_hashmap_t *map);
void         fe_hashmap_kill(fe_hashmap_t *map);
void         fe_hashmap_add(fe_hashmap_t *map, const char *key, u64 val);
u64          fe_hashmap_del(fe_hashmap_t *map, const char *key);
u64          fe_hashmap_del_at(fe_hashmap_t *map, u64 index);
u64          fe_hashmap_get(const fe_hashmap_t *map, const char *key);
u64          fe_hashmap_get_at(const fe_hashmap_t *map, u64 index);
const char*  fe_hashmap_get_key_at(const fe_hashmap_t *map, u64 index);
const char*  fe_hashmap_get_key_for_value(const fe_hashmap_t *map, u64 val);
u64          fe_hashmap_get_capacity(const fe_hashmap_t *map);
u64          fe_hashmap_get_count(const fe_hashmap_t *map);
fe_hashmap_t fe_hashmap_dup(const fe_hashmap_t *map);
void         fe_hashmap_print(const fe_hashmap_t *map);
