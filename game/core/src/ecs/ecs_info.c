#include <core/ecs/ecs_info.h>

static void
register_components(void)
{
    fe_ecs_register_component(CO_COMPONENT_PLAYER_INPUT, sizeof(co_player_input_t), NULL);
}

static void
register_queries(void)
{
    fe_ecs_register_query(CO_QUERY_PLAYER_INPUTS, FE_COMPONENT_TRANSFORM, FE_COMPONENT_RIGIDBODY, CO_COMPONENT_PLAYER_INPUT);
}


static void
register_fixed_tick_systems(void)
{
    fe_ecs_register_system(co_system_player_input_handle_fixed,                     FE_SYSTEM_FIXED_TICK);

    fe_ecs_register_system(fe_core_system_transform_view_update_previous_fixed,     FE_SYSTEM_FIXED_TICK);
    fe_ecs_register_system(fe_core_system_skeleton_move_bones,                      FE_SYSTEM_FIXED_TICK);
    fe_ecs_register_system(fe_core_system_transform_generate_world_transform_fixed, FE_SYSTEM_FIXED_TICK);
    fe_ecs_register_system(fe_core_system_skeleton_apply_bone_offset_matrix,        FE_SYSTEM_FIXED_TICK);


    fe_ecs_register_system(fe_core_system_rigidbody_handle_collisions,              FE_SYSTEM_FIXED_TICK);
    fe_ecs_register_system(fe_core_system_collision_handle_simple_colliders,        FE_SYSTEM_FIXED_TICK);
}

static void
register_tick_systems(void)
{
    fe_ecs_register_system(fe_core_system_skeleton_tick_animation,           FE_SYSTEM_TICK);

    fe_ecs_register_system(fe_core_system_transform_view_generate_world_matrix, FE_SYSTEM_TICK);
}

static void
register_systems(void)
{
    register_tick_systems();
    register_fixed_tick_systems();
}

void 
co_ecs_setup(void)
{
    register_components();
    register_queries();
    register_systems();
}

void 
co_ecs_cleanup(void)
{

}


