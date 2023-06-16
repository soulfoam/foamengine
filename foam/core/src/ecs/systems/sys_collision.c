#include <foam/core/ext/sugar.h>
#include <foam/core/ext/vector.h>
#include <foam/core/ecs/ecs_info.h>

static bool
contains_collision(fe_vec(fe_entity_id) collisions, fe_entity_id id)
{
    if (!collisions) return false;

    for (u64 i = 0; i < fe_vec_get_count(collisions); i++)
    {
        if (FE_HANDLE_EQ(collisions[i], id)) return true;
    }

    return false;
}

static void
handle_simple_collision_callbacks(void)
{
    fe_ecs_query(FE_QUERY_SIMPLE_COLLIDERS, e)
    {       
        fe_simple_collider_t *col = fe_entity_get_component(*e, FE_COMPONENT_SIMPLE_COLLIDER);

        for (u64 i = 0; i < fe_vec_get_count(col->prev_collisions); i++)
        {
            fe_entity_id prev_id = col->prev_collisions[i];
            if (!contains_collision(col->collisions, prev_id))
            {
                if (col->collision_exit_fn) col->collision_exit_fn(*e, prev_id);
            }
        }

        for (u64 i = 0; i < fe_vec_get_count(col->collisions); i++)
        {
            fe_entity_id id = col->collisions[i];
            if (!contains_collision(col->prev_collisions, id))
            {
                if (col->collision_enter_fn) col->collision_enter_fn(*e, id);
            }
            else
            {
                if (col->collision_fn) col->collision_fn(*e, id);
            }
        }
    }
}

static void
update_simple_collisions(void)
{
    fe_ecs_query(FE_QUERY_SIMPLE_COLLIDERS, e)
    {      
        fe_simple_collider_t *col = fe_entity_get_component(*e, FE_COMPONENT_SIMPLE_COLLIDER);
        fe_vec_clear(col->prev_collisions, NULL); 
        fe_vec_append(col->prev_collisions, col->collisions);
        fe_vec_clear(col->collisions, NULL); 
    }
}

static void
gather_simple_collisions(void)
{
    fe_vec(fe_entity_id) cols = fe_ecs_get_entities(FE_QUERY_SIMPLE_COLLIDERS);
    for (u64 i = 0; i < fe_vec_get_count(cols); i++)
    {
        fe_simple_collider_t *col = fe_entity_get_component(cols[i], FE_COMPONENT_SIMPLE_COLLIDER);
        fe_transform_t *col_xform = fe_entity_get_component(cols[i], FE_COMPONENT_TRANSFORM);
        fe_aabb_t col_aabb_ws;
        fe_aabb_init_in_world_space(&col_aabb_ws, &col->aabb.min, &col->aabb.max, fe_transform_get_world_translation(col_xform));
        for (u64 j = i; j < fe_vec_get_count(cols); j++)
        {     
            fe_simple_collider_t *other = fe_entity_get_component(cols[j], FE_COMPONENT_SIMPLE_COLLIDER);
            fe_transform_t *other_xform = fe_entity_get_component(cols[j], FE_COMPONENT_TRANSFORM);
            fe_aabb_t other_aabb_ws;
            fe_aabb_init_in_world_space(&other_aabb_ws, &other->aabb.min, &other->aabb.max, fe_transform_get_world_translation(other_xform)); 
            if (fe_aabb_intersects_aabb(&col_aabb_ws, &other_aabb_ws))
            {
                fe_vec_add(col->collisions, cols[j]);
                fe_vec_add(other->collisions, cols[i]);
            }
        }
        
    }
}

void 
fe_core_system_collision_handle_mesh_colliders(void)
{
//    fe_ecs_query(FE_QUERY_MESH_COLLIDERS, e)
//    {       
//        fe_mesh_collider_t *mc = fe_entity_get_component(*e, FE_COMPONENT_MESH_COLLIDER);
//        fe_transform_t *xform  = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);
//
//        if (xform->flags & FE_TRANSFORM_DIRTY_LOCAL) 
//        {
//            fe_mesh_apply_transform(&mc->mesh, &xform->world_trl, &xform->world_rot, &xform->world_scl);
//            fe_log("TRUE %s", fe_entity_get_name(*e));
//        }
//    }
}

void 
fe_core_system_collision_handle_simple_colliders(void)
{
    gather_simple_collisions();
    handle_simple_collision_callbacks();
    update_simple_collisions();
}
