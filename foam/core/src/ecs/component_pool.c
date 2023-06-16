#include <foam/core/ecs/component_pool.h>

void
fe_component_pool_make(fe_component_pool_t *pool, size_t size, fe_kill_func kill_func)
{
    pool->capacity  = 1;
    pool->data      = malloc(pool->capacity * size);
    pool->size      = size;
    pool->owner     = malloc(pool->capacity * sizeof(*pool->owner));
    pool->count     = 0;
    pool->kill_func = kill_func;

    for (size_t i = 0; i < pool->capacity; i++)
    {
        pool->owner[i] = FE_INVALID_ID;
    }
}

void
fe_component_pool_kill(fe_component_pool_t *pool)
{
    if (pool->kill_func)
    {
        for (size_t i = 0; i < pool->capacity; i++)
        {
            if (pool->owner[i] != FE_INVALID_ID)
            {
                pool->kill_func(fe_component_pool_get_data(pool, i));
            }
        }
    }

    free(pool->data);
    free(pool->owner);
}

void
fe_component_pool_clear(fe_component_pool_t *pool)
{
    for (size_t i = 0; i < pool->capacity; i++)
    {
        if (pool->owner[i] != FE_INVALID_ID)
        {
            if (pool->kill_func) pool->kill_func(fe_component_pool_get_data(pool, i));
            pool->owner[i] = FE_INVALID_ID;
        }
    }     
}

size_t
fe_component_pool_get_capacity(const fe_component_pool_t *pool)
{
    return pool->capacity;
}

size_t
fe_component_pool_get_count(const fe_component_pool_t *pool)
{
    return pool->count;
}

void
fe_component_pool_set_owner(fe_component_pool_t *pool, fe_component_pool_id id, fe_entity_id owner)
{
    pool->owner[id] = owner.id;
}

fe_entity_id
fe_component_pool_get_owner(fe_component_pool_t *pool, fe_component_pool_id id)
{
    return (fe_entity_id){pool->owner[id]};
}

void
fe_component_pool_set_data(fe_component_pool_t *pool, fe_component_pool_id id, void *data)
{
    u8 *ptr = ((u8*)(pool->data + (id * pool->size)));
    *ptr = *(u8*)data;
}

void*
fe_component_pool_get_data(const fe_component_pool_t *pool, fe_component_pool_id id)
{
    if (id >= pool->capacity || pool->owner[id] == FE_INVALID_ID) return NULL;
    return ((u8*)(pool->data + (id * pool->size)));
}

void*
fe_component_pool_get_data_ignore_owner(const fe_component_pool_t *pool, fe_component_pool_id id)
{
    if (id >= pool->capacity) return NULL;
    return ((u8*)(pool->data + (id * pool->size)));
}

fe_component_pool_id
fe_component_pool_add(fe_component_pool_t *pool, fe_entity_id entity_id)
{
    if (pool->count == pool->capacity)
    {  
        u64 new_capacity = pool->capacity * 2;
        void *new_data = realloc(pool->data, new_capacity * pool->size);
        if (new_data) pool->data = new_data;

        void *new_owner = realloc(pool->owner, new_capacity * sizeof(*pool->owner));
        if (new_owner) 
        { 
            pool->owner = new_owner;
            for (u64 i = new_capacity; i --> pool->capacity;)
            {
                pool->owner[i] = FE_INVALID_ID; 
            }
        }

        pool->capacity = new_capacity;
    }

    for (size_t i = 0; i < pool->capacity; i++)
    {
        if (pool->owner[i] == FE_INVALID_ID)
        {
            pool->count++;
            pool->owner[i] = entity_id.id;
            return i;
        }
    }

    fe_log_warn("FE_Memory pool is full!");

    return FE_INVALID_ID;
}

void
fe_component_pool_del(fe_component_pool_t *pool, fe_component_pool_id id)
{
    if (pool->owner[id] == FE_INVALID_ID) return;

    if (pool->kill_func) pool->kill_func(fe_component_pool_get_data(pool, id));

    pool->owner[id] = FE_INVALID_ID;
    pool->count--;
}

void
fe_component_pool_del_no_kill(fe_component_pool_t *pool, fe_component_pool_id id)
{
    if (pool->owner[id] == FE_INVALID_ID) return;

    pool->owner[id] = FE_INVALID_ID;
    pool->count--;
}

void
fe_component_pool_print(fe_component_pool_t *pool)
{
    for (size_t i = 0; i < pool->capacity; i++)
    {
        fe_log("[%u]: %u", i, pool->owner[i]);
    }
    printf("\n\n");
}

