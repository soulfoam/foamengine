#pragma once

#include <foam/core/ext/genvec.h>
#include <foam/core/ext/vector.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/logger.h>

FE_DEFINE_HANDLE(fe_entity_id);
typedef fe_genvec_id fe_component_id;
typedef u64 fe_component_pool_id;
typedef void (*fe_kill_func)();

typedef struct
{
	void *data;
	size_t capacity;
	size_t size;
    size_t count;

    u64 *owner;

    fe_kill_func kill_func;

} fe_component_pool_t;

void                 fe_component_pool_make(fe_component_pool_t *pool, size_t size, fe_kill_func kill_func);
void                 fe_component_pool_kill(fe_component_pool_t *pool);
void                 fe_component_pool_clear(fe_component_pool_t *pool);
void                 fe_component_pool_set_owner(fe_component_pool_t *pool, fe_component_pool_id id, fe_entity_id entity_id);
void                 fe_component_pool_set_data(fe_component_pool_t *pool, fe_component_pool_id id, void *data);
size_t               fe_component_pool_get_capacity(const fe_component_pool_t *pool);
size_t               fe_component_pool_get_count(const fe_component_pool_t *pool);
fe_entity_id         fe_component_pool_get_owner(fe_component_pool_t *pool, fe_component_pool_id id);
void*                fe_component_pool_get_data(const fe_component_pool_t *pool, fe_component_pool_id id);
void*                fe_component_pool_get_data_ignore_owner(const fe_component_pool_t *pool, fe_component_pool_id id);
fe_component_pool_id fe_component_pool_add(fe_component_pool_t *pool, fe_entity_id entity_id);
void                 fe_component_pool_del(fe_component_pool_t *pool, fe_component_pool_id);
void                 fe_component_pool_del_no_kill(fe_component_pool_t *pool, fe_component_pool_id id);
void                 fe_component_pool_print(fe_component_pool_t *pool);

