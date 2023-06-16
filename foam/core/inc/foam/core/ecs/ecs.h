#pragma once

#include <foam/core/ecs/component_pool.h>
#include <foam/core/ext/genvec.h>
#include <foam/core/ext/vector.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/logger.h>
#include <foam/core/config/config.h>

typedef void (*fe_ecs_system_func)();

typedef u64 fe_query_id;
typedef struct fe_entity_t fe_entity_t;

typedef u64 FE_COMPONENT_TYPE;
typedef u64 FE_QUERY_TYPE;
#define FE_ECS_ROOT fe_ecs_get_root()

typedef enum
{
    FE_SYSTEM_TICK,
    FE_SYSTEM_FIXED_TICK,
} FE_SYSTEM_TYPE;

typedef struct
{
    fe_ecs_system_func func;
    FE_SYSTEM_TYPE type;
} fe_system_t;

typedef void (*fe_entity_property_read_fn) (fe_entity_id ent_id, const fe_value_t *val);
typedef void (*fe_resource_property_read_fn) (const fe_value_t *val);

#define fe_ecs_for_each_entity(l, e)                                                                     \
for (u64 _fe_for_each_index = 0; (_fe_for_each_index == 0); ++_fe_for_each_index)             \
for (__typeof__(*l) e = *l; _fe_for_each_index < fe_vec_get_count(l); e=l[++_fe_for_each_index])

#define fe_ecs_query(query, e) fe_vec_for_each(fe_ecs_get_entities(query), e)
#define fe_ecs_query_index(query, e, i) fe_vec_for_each_index(fe_ecs_get_entities(query), e, i)

void fe_ecs_setup(u32 component_count, u32 query_count);
void fe_ecs_cleanup(void);
void fe_ecs_register_component(FE_COMPONENT_TYPE type, size_t comp_size, fe_kill_func kill_func);
#define fe_ecs_register_query(type, ...) fe_register_query(type, sizeof((FE_COMPONENT_TYPE[]){__VA_ARGS__}) / sizeof(FE_COMPONENT_TYPE), (FE_COMPONENT_TYPE[]){__VA_ARGS__})
void fe_register_query(FE_QUERY_TYPE query_type, u64 count, FE_COMPONENT_TYPE components[]);
void fe_ecs_register_system(fe_ecs_system_func func, FE_SYSTEM_TYPE type);
void fe_ecs_register_entity_property_read_callback(const char *property_name, fe_entity_property_read_fn cb);
void fe_ecs_register_resource_property_read_callback(const char *property_name, fe_resource_property_read_fn cb);
void fe_ecs_run_systems(FE_SYSTEM_TYPE type);

u64 fe_ecs_get_component_count(void);
u64 fe_ecs_get_query_count(void);


fe_entity_id fe_ecs_add_entity(const char *name);
fe_entity_id fe_ecs_add_child_entity(fe_entity_id parent_id, const char *name);
fe_entity_t* fe_ecs_get_entity(fe_entity_id ent_id);
bool         fe_ecs_is_valid_entity(fe_entity_id ent_id);
fe_entity_id fe_ecs_find_entity_id(fe_entity_id parent_id, const char *name);
void         fe_ecs_del_entity(fe_entity_id ent_id);
void*        fe_entity_add_component(fe_entity_id ent_id, FE_COMPONENT_TYPE type);
void         fe_entity_del_component(fe_entity_id ent_id, FE_COMPONENT_TYPE type);
void         fe_entity_enable_component(fe_entity_id ent_id, FE_COMPONENT_TYPE type);
void         fe_entity_disable_component(fe_entity_id ent_id, FE_COMPONENT_TYPE type);
bool         fe_entity_is_component_enabled(fe_entity_id ent_id, FE_COMPONENT_TYPE type);
void*        fe_entity_get_component(fe_entity_id ent_id, FE_COMPONENT_TYPE type);
bool         fe_entity_has_component(fe_entity_id ent_id, FE_COMPONENT_TYPE type);
bool         fe_entity_has_components(fe_entity_id ent_id, u64 count, FE_COMPONENT_TYPE type[]);
void         fe_entity_set_name(fe_entity_id ent_id, const char *name);
bool         fe_entity_set_parent(fe_entity_id child_id, fe_entity_id parent_id);
const char*   fe_entity_get_name(fe_entity_id ent_id);
fe_entity_id  fe_entity_get_parent(fe_entity_id ent_id);
fe_entity_id* fe_entity_get_children(fe_entity_id ent_id);

fe_entity_id* fe_ecs_get_entities(FE_QUERY_TYPE query);
fe_entity_id  fe_ecs_get_root(void);

u32 fe_ecs_get_entity_count(void);

fe_entity_id fe_ecs_add_entity_from_file(const char *path);

void fe_ecs_print_entity(fe_entity_id ent_id);
void fe_ecs_print_queries(void);
void fe_ecs_print_query(FE_QUERY_TYPE type);

