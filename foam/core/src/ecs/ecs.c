#include <foam/core/ecs/ecs.h>
#include <foam/core/ext/hashmap.h>
#include <foam/core/config/config.h>

struct
fe_entity_t
{
    char                         *name;
    fe_vec(fe_component_pool_id) components;
    fe_vec(bool)                 enabled_components;
    fe_entity_id                 parent;
    fe_vec(fe_entity_id)         children;
};

typedef struct
{
    fe_vec(bool)         components;
    fe_vec(fe_entity_id) entities;
} fe_query_t;

typedef struct
{	
    fe_genvec(fe_entity_t)      entities;

    fe_vec(fe_component_pool_t) components;
    fe_vec(fe_query_t)          queries;
    fe_vec(fe_system_t)         systems;

    fe_vec(fe_entity_property_read_fn)   entity_prop_callbacks;
    fe_hashmap_t                         entity_prop_lookup;

    fe_vec(fe_resource_property_read_fn) resource_prop_callbacks;
    fe_hashmap_t                         resource_prop_lookup;
    
    fe_entity_id root_id;
} fe_ecs_t;

static fe_ecs_t ecs;

static fe_entity_id
ecs_make_entity(const char *name, fe_entity_id parent_id)
{
    fe_entity_id e_id = (fe_entity_id){FE_INVALID_ID};
    fe_entity_t *e    = NULL;
    fe_genvec_add_and_get(ecs.entities, e_id.id, e);

    e->name = (name ? strdup(name) : NULL);
    fe_vec_make_with_count(e->components,         fe_ecs_get_component_count());
    fe_vec_make_with_count(e->enabled_components, fe_ecs_get_component_count());
    for (u64 i = 0; i < fe_ecs_get_component_count(); i++)
    {
        e->components[i]         = FE_INVALID_ID;
        e->enabled_components[i] = false;
    }

    e->children = FE_VEC_EMPTY;
    e->parent   = parent_id;

    fe_entity_t *parent = fe_ecs_get_entity(parent_id);
    if (parent)
    {
        fe_vec_add(parent->children, e_id);
    }

    return e_id;
}

static void
ecs_kill_entity(fe_entity_t *ent)
{
    free(ent->name);
    fe_vec_kill(ent->components, NULL);
    fe_vec_kill(ent->enabled_components, NULL);
    fe_vec_kill(ent->children, NULL);
}

static void
ecs_kill_query(fe_query_t *query)
{ 
    fe_vec_kill(query->components, NULL);
    fe_vec_kill(query->entities, NULL);
}

static bool
ecs_entity_name_eq(fe_entity_t *ent, const char *str)
{
    if (!ent || !ent->name || STR_NOT_EQ(ent->name, str)) return false;

    return true;
}

void
fe_ecs_setup(u32 component_count, u32 query_count)
{
    ecs.entities = FE_GENVEC_EMPTY;
    fe_vec_make_with_count(ecs.components, component_count);
    fe_vec_make_with_count(ecs.queries,    query_count);

    for (u64 i = 0; i < fe_ecs_get_query_count(); i++) 
    {
        fe_query_t *query = &ecs.queries[i];
        fe_vec_make_with_count(query->components, component_count);
        for (u64 i = 0; i < fe_ecs_get_component_count(); i++)
        {
            query->components[i] = false;
        }

        query->entities = FE_VEC_EMPTY;
    }


    for (u64 i = 0; i < fe_ecs_get_component_count(); i++) 
    {
        ecs.components[i].data = NULL;
    }

    ecs.systems = FE_VEC_EMPTY;

    ecs.entity_prop_callbacks = FE_VEC_EMPTY;
    fe_hashmap_make(&ecs.entity_prop_lookup);

    ecs.resource_prop_callbacks = FE_VEC_EMPTY;
    fe_hashmap_make(&ecs.resource_prop_lookup);

    ecs.root_id = ecs_make_entity(NULL, (fe_entity_id){FE_INVALID_ID});
}

void
fe_ecs_cleanup(void)
{
    fe_vec_kill(ecs.systems, NULL);
    fe_vec_kill(ecs.queries, ecs_kill_query);

    fe_genvec_kill(ecs.entities, ecs_kill_entity);

    fe_vec_kill(ecs.components, fe_component_pool_kill);

    fe_vec_kill(ecs.entity_prop_callbacks, NULL);
    fe_hashmap_kill(&ecs.entity_prop_lookup);

    fe_vec_kill(ecs.resource_prop_callbacks, NULL);
    fe_hashmap_kill(&ecs.resource_prop_lookup);
}

void
fe_ecs_register_component(FE_COMPONENT_TYPE type, size_t comp_size, fe_kill_func kill_func)
{
    fe_component_pool_t *pool = &ecs.components[type];
    fe_component_pool_make(pool, comp_size, kill_func);
}

void
fe_register_query(FE_QUERY_TYPE query_type, u64 count, FE_COMPONENT_TYPE components[])
{
    for (u64 i = 0; i < count; i++)
    {
        ecs.queries[query_type].components[components[i]] = true;
    }
}

void          
fe_ecs_register_system(fe_ecs_system_func func, FE_SYSTEM_TYPE type)
{
    fe_system_t *sys = NULL;
    fe_vec_add_and_get(ecs.systems, sys);
    sys->func = func;
    sys->type = type;
}

void
fe_ecs_register_entity_property_read_callback(const char *property_name, fe_entity_property_read_fn cb)
{
    fe_vec_add(ecs.entity_prop_callbacks, cb);
    fe_hashmap_add(&ecs.entity_prop_lookup, property_name, fe_vec_get_last_index(ecs.entity_prop_callbacks));
}

fe_entity_property_read_fn
fe_ecs_get_entity_property_read_callback(const char *name)
{
    u64 index = fe_hashmap_get(&ecs.entity_prop_lookup, name);
    if (index == FE_INVALID_INDEX) return NULL;

    return ecs.entity_prop_callbacks[index];
}

void
fe_ecs_register_resource_property_read_callback(const char *property_name, fe_resource_property_read_fn cb)
{
    fe_vec_add(ecs.resource_prop_callbacks, cb);
    fe_hashmap_add(&ecs.resource_prop_lookup, property_name, fe_vec_get_last_index(ecs.resource_prop_callbacks));
}

fe_resource_property_read_fn
fe_ecs_get_resource_property_read_callback(const char *name)
{
    u64 index = fe_hashmap_get(&ecs.resource_prop_lookup, name);
    if (index == FE_INVALID_INDEX) return NULL;

    return ecs.resource_prop_callbacks[index];
}

void
fe_ecs_run_systems(FE_SYSTEM_TYPE type)
{
    FE_TODO("Time how long a system takes to run for easy optimization");

    fe_vec_for_each(ecs.systems, sys)
    {
        if (sys->type == type)
        {
            //f64 now = SDL_GetPerformanceCounter();

            sys->func();

            //f64 time = (f64)((SDL_GetPerformanceCounter() - now)*1000 / (f64)SDL_GetPerformanceFrequency());
            //fe_log("Sys [%u] Type [%u] : %f ms", i, type, time);
        }
    }

}

fe_entity_id
fe_ecs_add_entity(const char *name)
{
    return ecs_make_entity(name, ecs.root_id);
}

fe_entity_id
fe_ecs_add_child_entity(fe_entity_id parent_id, const char *name)
{
    fe_entity_t *parent = fe_ecs_get_entity(parent_id);
    if (!parent) return (fe_entity_id){FE_INVALID_ID};

    return ecs_make_entity(name, parent_id);
}

void
fe_ecs_del_entity(fe_entity_id ent_id)
{	
    fe_entity_t *ent = fe_genvec_get(ecs.entities, ent_id.id);

    if (!ent) 
    {
        fe_log_warn("Failed to delete non-existent entity %u", ent_id);
        return;
    }
    
    for (u64 i = 0; i < fe_ecs_get_component_count(); i++)
    {
        if (ent->components[i] == FE_INVALID_ID) continue;

        fe_entity_del_component(ent_id, i);
    }

    fe_vec_for_each(ent->children, id)
    {
        fe_ecs_del_entity(*id);
    }

    if (!FE_HANDLE_EQ(ent_id, ecs.root_id)) fe_genvec_del(ecs.entities, ent_id.id, ecs_kill_entity);
}

fe_entity_id
fe_ecs_find_entity_id(fe_entity_id parent_id, const char *name)
{
    fe_entity_t *parent = fe_ecs_get_entity(parent_id);
    if (!parent) return (fe_entity_id){FE_INVALID_ID};
    if (ecs_entity_name_eq(fe_ecs_get_entity(parent_id), name)) return parent_id;

    fe_vec_for_each(parent->children, child)
    {
        fe_entity_id ret_id = fe_ecs_find_entity_id(*child, name);
        if (!FE_INVALID_HANDLE_ID(ret_id)) return ret_id;
    }

    return (fe_entity_id){FE_INVALID_ID};
}

bool
fe_entity_has_component(fe_entity_id ent_id, FE_COMPONENT_TYPE type)
{
    fe_entity_t *ent = fe_genvec_get(ecs.entities, ent_id.id);
    if (!ent) return false;

    return ent->components[type] != FE_INVALID_ID;
}

bool
fe_entity_has_components(fe_entity_id ent_id, u64 count, FE_COMPONENT_TYPE type[])
{
    for (u64 i = 0; i < count; i++)
    {
        if (!fe_entity_has_component(ent_id, type[i])) return false;
    }

    return true;
}

static void
entity_handle_query_addition(fe_entity_id ent_id)
{ 
    fe_entity_t *ent = fe_ecs_get_entity(ent_id);
    if (!ent) return;

    for (u64 i = 0; i < fe_ecs_get_query_count(); i++)
    {
        fe_query_t *query = &ecs.queries[i];
        bool add_to_query = true;
        for (u64 c = 0; c < fe_ecs_get_component_count(); c++)
        {
            if (query->components[c] && (ent->components[c] == FE_INVALID_ID || !ent->enabled_components[c]))
            {
                add_to_query = false;
                break;
            }
        }

        if (add_to_query)
        {
            bool exists = false;
            fe_vec_for_each(query->entities, e)
            {
                if (FE_HANDLE_EQ((*e), ent_id))
                {
                    exists = true;
                    break;
                }
            }

            if (!exists) fe_vec_add(query->entities, ent_id);
        }
    }
}

static void
entity_handle_query_deletion(fe_entity_id ent_id)
{
    fe_entity_t *ent = fe_ecs_get_entity(ent_id);
    if (!ent) return;

    for (u64 i = 0; i < fe_ecs_get_query_count(); i++)
    {
        fe_query_t *query = &ecs.queries[i];
        fe_vec_for_each_index(query->entities, e_id, ec)
        {
            if (!FE_HANDLE_EQ((*e_id), ent_id)) continue;

            for (u64 c = 0; c < fe_ecs_get_component_count(); c++)
            {
                if (query->components[c] && (ent->components[c] == FE_INVALID_ID || !ent->enabled_components[c]))
                {
                    fe_vec_del(query->entities, ec, NULL);
                }
            }
        }

    }
}

void*
fe_entity_add_component(fe_entity_id ent_id, FE_COMPONENT_TYPE type)
{
    fe_entity_t *ent = fe_ecs_get_entity(ent_id);
    if (!ent) return NULL;

    if (fe_entity_has_component(ent_id, type)) 
    {
        fe_log_warn("Add component failed because entity %u (%s) already has component %u", ent_id, ent->name ? ent->name : "", type);
        return NULL;
    }

    fe_component_pool_id id = fe_component_pool_add(&ecs.components[type], ent_id);
    ent->components[type] = id;
    ent->enabled_components[type] = true;
    
    entity_handle_query_addition(ent_id);

    return fe_component_pool_get_data(&ecs.components[type], id);
}

void
fe_entity_del_component(fe_entity_id ent_id, FE_COMPONENT_TYPE type)
{
    fe_entity_t *ent = fe_ecs_get_entity(ent_id);
    if (!ent) return;

    if (!fe_entity_has_component(ent_id, type)) 
    {
        fe_log_warn("Delete component failed because entity %u (%s) doesn't have component %u", ent_id, ent->name ? ent->name : "", type);
        return;
    }

    fe_component_pool_del(&ecs.components[type], ent->components[type]);
    ent->components[type] = FE_INVALID_ID;
    ent->enabled_components[type] = false;

    entity_handle_query_deletion(ent_id);
}

void
fe_entity_enable_component(fe_entity_id ent_id, FE_COMPONENT_TYPE type)
{
    fe_entity_t *ent = fe_ecs_get_entity(ent_id);
    if (!ent) return;

    ent->enabled_components[type] = true;
    entity_handle_query_addition(ent_id);
}

void
fe_entity_disable_component(fe_entity_id ent_id, FE_COMPONENT_TYPE type)
{
    fe_entity_t *ent = fe_ecs_get_entity(ent_id);
    if (!ent) return;

    ent->enabled_components[type] = false;
    entity_handle_query_deletion(ent_id);
}

bool         
fe_entity_is_component_enabled(fe_entity_id ent_id, FE_COMPONENT_TYPE type)
{
    fe_entity_t *ent = fe_ecs_get_entity(ent_id);
    if (!ent) return false;

    return ent->enabled_components[type];
}

void*
fe_entity_get_component(fe_entity_id ent_id, FE_COMPONENT_TYPE type)
{
    fe_entity_t *ent = fe_ecs_get_entity(ent_id);
    if (!ent) return NULL;

    return fe_component_pool_get_data(&ecs.components[type], ent->components[type]);
}

void
fe_entity_set_name(fe_entity_id ent_id, const char *name)
{
    fe_entity_t *ent = fe_ecs_get_entity(ent_id);
    if (!ent) return;

    if (ent->name) free(ent->name);
    ent->name = strdup(name);
}

bool
fe_entity_set_parent(fe_entity_id child_id, fe_entity_id parent_id)
{ 
    fe_entity_t *parent = fe_ecs_get_entity(parent_id);
    fe_entity_t *child = fe_ecs_get_entity(child_id);

    if (!parent || !child || FE_HANDLE_EQ(child->parent, parent_id)) return false;

    fe_entity_t *child_cur_parent = fe_ecs_get_entity(child->parent);
    if (child_cur_parent)
    {
        fe_vec_for_each_index(child_cur_parent->children, c, i)
        {
            if (FE_HANDLE_EQ((*c), child_id))
            {
                fe_vec_del(child_cur_parent->children, i, NULL);
                break;
            }
        } 
    }

    fe_vec_add(parent->children, child_id);
    child->parent = parent_id;
    
    return true;
}

const char*   
fe_entity_get_name(fe_entity_id ent_id)
{
    fe_entity_t *ent = fe_ecs_get_entity(ent_id);
    if (!ent) return NULL;

    return ent->name;
}

fe_entity_id  
fe_entity_get_parent(fe_entity_id ent_id)
{
    fe_entity_t *ent = fe_ecs_get_entity(ent_id);
    if (!ent) return (fe_entity_id){FE_INVALID_ID};

    return ent->parent;
}

fe_entity_id* 
fe_entity_get_children(fe_entity_id ent_id)
{
    fe_entity_t *ent = fe_ecs_get_entity(ent_id);
    if (!ent) return NULL;

    return ent->children;
}

fe_entity_t*
fe_ecs_get_entity(fe_entity_id ent_id)
{
    return fe_genvec_get(ecs.entities, ent_id.id);
}

bool         
fe_ecs_is_valid_entity(fe_entity_id ent_id)
{
    return fe_ecs_get_entity(ent_id);
}

fe_entity_id* 
fe_ecs_get_entities(FE_QUERY_TYPE query)
{
    return ecs.queries[query].entities;
}

fe_entity_id
fe_ecs_get_root(void)
{
    return ecs.root_id;
}

u64
fe_ecs_get_component_count(void)
{
    return fe_vec_get_count(ecs.components);
}

u64
fe_ecs_get_query_count(void)
{
    return fe_vec_get_count(ecs.queries);
}

u32        
fe_ecs_get_entity_count(void)
{
    return fe_genvec_get_count(ecs.entities);
}

static fe_entity_id
handle_entity_callbacks(fe_entity_id ent_id, const fe_config_object_t *obj)
{
    for (u64 i = 0; i < fe_config_object_get_property_count(obj); i++)
    {
        const fe_config_property_t *prop = fe_config_object_get_property_at(obj, i);
        fe_entity_property_read_fn cb = fe_ecs_get_entity_property_read_callback(prop->name);
        if (cb) cb(ent_id, &prop->value);
    }

    return ent_id;
}

static void
handle_resource_callbacks(const fe_config_object_t *obj)
{
    for (u64 i = 0; i < fe_config_object_get_property_count(obj); i++)
    {
        const fe_config_property_t *prop = fe_config_object_get_property_at(obj, i);
        fe_resource_property_read_fn cb = fe_ecs_get_resource_property_read_callback(prop->name);
        if (cb) cb(&prop->value);
    }
}

fe_entity_id
fe_ecs_add_entity_from_file(const char *path)
{ 
    fe_config_t scn;
    if (fe_config_make_from_file(&scn, path) != FE_OK)
    {
        fe_log_error("Failed to load entity config '%s'", path);
        return (fe_entity_id){FE_INVALID_ID};
    }

    const fe_config_object_t *root_ent_obj = fe_config_get_object(&scn, "ROOT_ENTITY");
    if (!root_ent_obj)
    {
        fe_log_error("Config '%s' must contain 'ROOT_ENTITY' object", path);
        return (fe_entity_id){FE_INVALID_ID};
    }

    const char *root_ent_name = FE_CFG_PROP_AS_STRING(root_ent_obj, "name");
    fe_entity_id root_ent_id = fe_ecs_add_entity(root_ent_name);

    const fe_config_object_t *resources_obj = fe_config_get_object(&scn, "RESOURCES");
    if (resources_obj) handle_resource_callbacks(resources_obj);

    typedef struct
    {
        const fe_config_object_t *obj;
        fe_entity_id ent_id;
    } obj_id_pair;

    fe_vec(obj_id_pair) pairs = FE_VEC_EMPTY;

    for (u64 i = 0; i < fe_config_get_object_count(&scn); i++)
    {
        const fe_config_object_t *obj = fe_config_get_object_at(&scn, i);
        const char *obj_name          = fe_config_object_get_name(obj);
        if (STR_EQ(obj_name, "ENTITY"))
        {
            const char *ent_name = FE_CFG_PROP_AS_STRING(obj, "name");
            fe_entity_id ent_id = fe_ecs_add_child_entity(root_ent_id, ent_name);
            fe_vec_add(pairs, ((obj_id_pair){obj, ent_id}));
        }
    }

    handle_entity_callbacks(root_ent_id, root_ent_obj);
    fe_vec_for_each(pairs, p)
    {
        handle_entity_callbacks(p->ent_id, p->obj);
    }

    fe_vec_kill(pairs, NULL);
    fe_config_kill(&scn);

    return root_ent_id;
}

void
fe_ecs_print_entity(fe_entity_id ent_id)
{
    fe_entity_t *ent = fe_ecs_get_entity(ent_id);
    if (!ent) return;

    fe_entity_t *parent = fe_ecs_get_entity(ent->parent);
    const char *parent_name = (parent ? (parent->name ? parent->name : "") : "");

    fe_log("---- '%s' (%u) ---- Parent: '%s' (%u)", (ent->name ? ent->name : ""), ent_id, parent_name, ent->parent);
    for (u32 i = 0; i < fe_ecs_get_component_count(); i++)
    {
        fe_log("Component [%u]: %u", i, ent->components[i]);
    }
    fe_vec_for_each(ent->children, child)
    {
        fe_ecs_print_entity(*child);
    }
}

void
fe_ecs_print_query(FE_QUERY_TYPE type)
{
    fe_log("-- Query [%u] --", type);
    fe_query_t *q = &ecs.queries[type];
    for (u32 c = 0; c < fe_ecs_get_component_count(); c++)
    {
        fe_log("Component [%u]", q->components[c]);
    }
    for (u32 e = 0; e < fe_vec_get_count(q->entities); e++)
    {
        fe_entity_t *ent = fe_ecs_get_entity(q->entities[e]);
        fe_log("Entity [%s | %u]", (ent->name ? ent->name : "Unnamed"), q->entities[e]);
    }

}

void
fe_ecs_print_queries(void)
{
    fe_log("---- Queries (%u) ----", fe_ecs_get_query_count());
    for (u32 i = 0; i < fe_ecs_get_query_count(); i++)
    {
        fe_ecs_print_query(i);
    }
}
