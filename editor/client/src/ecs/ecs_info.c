#include <client/ecs/ecs_info.h>

static void
register_components(void)
{
    fe_ecs_register_component(EDITOR_COMPONENT_CAMERA, sizeof(ed_camera_t), NULL);
    fe_ecs_register_component(EDITOR_COMPONENT_EDITABLE, sizeof(ed_editable_t), NULL);
}

static void
register_queries(void)
{
    fe_ecs_register_query(EDITOR_QUERY_EDITABLES, EDITOR_COMPONENT_EDITABLE, FE_COMPONENT_TRANSFORM);
}

static void
register_core_fixed_tick_systems(void)
{
    fe_ecs_register_system(fe_core_system_transform_view_update_previous_fixed,     FE_SYSTEM_FIXED_TICK);
    fe_ecs_register_system(fe_core_system_skeleton_move_bones,                      FE_SYSTEM_FIXED_TICK);
    fe_ecs_register_system(fe_core_system_transform_generate_world_transform_fixed, FE_SYSTEM_FIXED_TICK);
    fe_ecs_register_system(fe_core_system_skeleton_apply_bone_offset_matrix,        FE_SYSTEM_FIXED_TICK);


    fe_ecs_register_system(fe_core_system_rigidbody_handle_collisions,              FE_SYSTEM_FIXED_TICK);
    fe_ecs_register_system(fe_core_system_collision_handle_simple_colliders,        FE_SYSTEM_FIXED_TICK);
}

static void
register_core_tick_systems(void)
{
    fe_ecs_register_system(fe_core_system_skeleton_tick_animation,           FE_SYSTEM_TICK);

    fe_ecs_register_system(fe_core_system_transform_view_generate_world_matrix, FE_SYSTEM_TICK);
}


static void
register_fixed_tick_systems(void)
{
    fe_ecs_register_system(ed_system_input_manager_fixed_tick, FE_SYSTEM_FIXED_TICK);

    fe_ecs_register_system(ed_system_editable_fixed_tick, FE_SYSTEM_FIXED_TICK);

    fe_ecs_register_system(ed_system_camera_fixed_tick, FE_SYSTEM_FIXED_TICK);

}

static void
register_tick_systems(void)
{
    fe_ecs_register_system(ed_system_camera_tick_pre, FE_SYSTEM_TICK);

    fe_ecs_register_system(fe_client_system_sprite3d_draw,             FE_SYSTEM_TICK);
    fe_ecs_register_system(fe_client_system_mesh_instance_draw_meshes, FE_SYSTEM_TICK);
    fe_ecs_register_system(fe_client_system_light_draw_lights,         FE_SYSTEM_TICK);
    fe_ecs_register_system(fe_client_system_fog_draw,                  FE_SYSTEM_TICK);
    fe_ecs_register_system(fe_client_system_sprite2d_draw,             FE_SYSTEM_TICK);
    fe_ecs_register_system(fe_client_system_text2d_draw,               FE_SYSTEM_TICK);

    fe_ecs_register_system(ed_system_input_manager_tick,              FE_SYSTEM_TICK);

    fe_ecs_register_system(ed_system_camera_tick_post, FE_SYSTEM_TICK);

}

static void
register_systems(void)
{
    register_core_fixed_tick_systems();
    register_core_tick_systems();

    register_fixed_tick_systems();
    register_tick_systems();
}

void 
editor_ecs_setup(void)
{
    register_components();
    register_queries();
    register_systems();

    ed_manager_setup(); 
    ed_input_manager_setup(); 
}

void 
editor_ecs_cleanup(void)
{
    ed_manager_cleanup(); 
    ed_input_manager_cleanup(); 
}
