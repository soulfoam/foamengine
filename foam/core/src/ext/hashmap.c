#include <foam/core/ext/sugar.h>
#include <foam/core/ext/io.h>
#include <foam/core/ext/hashmap.h>

//capacity must be a power of 2 
#define DEFAULT_HASHMAP_CAPACITY 32

static bool
is_null_item(fe_hashmap_item_t *item)
{
    return item->key == NULL;
}

static void
item_set_values(fe_hashmap_item_t *item, const char *key, u64 val)
{
    item->key = (key == NULL) ? NULL : strdup(key);
    item->val = val;
}

static u64
remove_item(fe_hashmap_t *map, u64 index)
{
    fe_hashmap_item_t *item = &map->items[index];
    u64 r                = item->val;
    if (item->key) free(item->key);
    item_set_values(item, NULL, FE_INVALID_INDEX);
    map->item_count--;
    return r;
}

static u64 
hash_index(u64 capacity, const char *key) 
{
    u64 hash, i;
    for (hash = i = 0; i < strlen(key); ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return (hash & (capacity-1)); 
}

static bool
hashmap_insert_into_first_available(fe_hashmap_item_t *items, u64 capacity, const char *key, u32 val)
{
    u64 index = hash_index(capacity, key);
    u64 checked = 0;
    bool exists = false;
    while(!is_null_item(&items[index]))
    {
        if (STR_EQ(items[index].key, key))
        {
            items[index].val = val;
            exists = true;
            break;
        }
        ++index;
        ++checked;
        index = (index & (capacity-1));

        if (checked == capacity) break;
    }

    if (!exists) item_set_values(&items[index], key, val);

    return !exists;
}

static void
hashmap_make(fe_hashmap_t *map, u64 capacity)
{
    map->capacity   = capacity;
    map->items      = malloc(sizeof(*map->items) * map->capacity);
    map->item_count = 0;

    for (u64 i = 0; i < map->capacity; i++)
    {
        fe_hashmap_item_t *item = &map->items[i];
        item_set_values(item, NULL, FE_INVALID_INDEX);
    }
}

static void
hashmap_expand(fe_hashmap_t *map)
{
    fe_hashmap_t new_map;
    hashmap_make(&new_map, map->capacity * 2);

    for (u64 i = 0; i < map->capacity; i++)
    {
        fe_hashmap_item_t *item = &map->items[i];
        if (is_null_item(item)) continue;

        fe_hashmap_add(&new_map, item->key, item->val);
    }

    fe_hashmap_kill(map);
    *map = new_map;
}

void
fe_hashmap_make(fe_hashmap_t *map)
{
    hashmap_make(map, DEFAULT_HASHMAP_CAPACITY);
}

void
fe_hashmap_kill(fe_hashmap_t *map)
{
    for (u64 i = 0; i < map->capacity; i++)
    {
        remove_item(map, i);
    }
    free(map->items);
}

void
fe_hashmap_add(fe_hashmap_t *map, const char *key, u64 val)
{
    if (key == NULL) return;

    if (map->item_count == map->capacity)
    {
        hashmap_expand(map);
    }

    if (hashmap_insert_into_first_available(map->items, map->capacity, key, val))
    {
        map->item_count++;
    }
}

u64
fe_hashmap_del(fe_hashmap_t *map, const char *key)
{ 
    u64 index = hash_index(map->capacity, key);

    u64 checked = 0;

    while(1)
    {
        if (!is_null_item(&map->items[index]) && STR_EQ(map->items[index].key, key)) 
        {
            u64 data = map->items[index].val;
            remove_item(map, index);
            return data;
        } 

        ++index;
        ++checked;
        index = (index & (map->capacity-1));

        if (checked == map->capacity) break;
    }

    //fe_log_warn("fe_hashmap_del couldn't find: %s", key);
    
    return FE_INVALID_INDEX;
}

u64
fe_hashmap_del_at(fe_hashmap_t *map, u64 index)
{
    return remove_item(map, index);
}

u64
fe_hashmap_get(const fe_hashmap_t *map, const char *key)
{
    u64 index = hash_index(map->capacity, key);

    u64 checked = 0;
    while(1)
    {
        if (!is_null_item(&map->items[index]) && STR_EQ(map->items[index].key, key))
        {
            return map->items[index].val; 
        }

        ++index;
        ++checked;
        index = (index & (map->capacity-1));

        if (checked == map->capacity) break;
    }

    //fe_log_warn("fe_hashmap_get couldn't find: %s", key);

    return FE_INVALID_INDEX;
}

u64
fe_hashmap_get_at(const fe_hashmap_t *map, u64 index)
{
    return map->items[index].val;
}

const char*
fe_hashmap_get_key_at(const fe_hashmap_t *map, u64 index)
{
    return map->items[index].key;

}

const char*
fe_hashmap_get_key_for_value(const fe_hashmap_t *map, u64 val)
{
    for (u64 i = 0; i < fe_hashmap_get_capacity(map); i++)
    {
        u64 v = fe_hashmap_get_at(map, i);    
        if (val == v) return fe_hashmap_get_key_at(map, i);
    }

    return NULL;
}

u64   
fe_hashmap_get_capacity(const fe_hashmap_t *map)
{
    return map->capacity;
}

u64   
fe_hashmap_get_count(const fe_hashmap_t *map)
{
    return map->item_count;
}

fe_hashmap_t
fe_hashmap_dup(const fe_hashmap_t *map)
{
    fe_hashmap_t hm;
    fe_hashmap_make(&hm);
    for (u64 i = 0; i < map->capacity; i++)
    {
        fe_hashmap_item_t *item = &map->items[i];
        if (!is_null_item(item))
        {
            fe_hashmap_add(&hm, item->key, item->val);
        }
    }
    return hm;
}

void
fe_hashmap_print(const fe_hashmap_t *map)
{
    fe_log("Hashmap [%p]: Count: (%u) | Capacity: (%u)", map, map->item_count, map->capacity);
    for (u64 i = 0; i < map->capacity; i++)
    {
        if (!is_null_item(&map->items[i]))
        {
            fe_log("Key: '%s' | Value: '%u'", map->items[i].key, map->items[i].val);
        }

    }
}

