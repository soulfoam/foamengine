#include <client/ecs/ecs_info.h>

static void
register_components(void)
{
    fe_ecs_register_component(CL_COMPONENT_FPS_CAMERA, sizeof(cl_fps_camera_t), NULL);
}

static void
register_queries(void)
{

}

static void
register_fixed_tick_systems(void)
{
    fe_ecs_register_system(cl_system_input_manager_fixed_tick, FE_SYSTEM_FIXED_TICK);

    fe_ecs_register_system(cl_system_fps_camera_fixed_tick, FE_SYSTEM_FIXED_TICK);
    fe_ecs_register_system(cl_system_fps_camera_weapon_fixed_tick, FE_SYSTEM_FIXED_TICK);

    fe_ecs_register_system(cl_system_debug_manager_fixed_tick, FE_SYSTEM_FIXED_TICK);
}

static void
register_tick_systems(void)
{
    fe_ecs_register_system(cl_system_fps_camera_tick_pre, FE_SYSTEM_TICK);

    fe_ecs_register_system(fe_client_system_sprite3d_draw,             FE_SYSTEM_TICK);
    fe_ecs_register_system(fe_client_system_mesh_instance_draw_meshes, FE_SYSTEM_TICK);
    fe_ecs_register_system(fe_client_system_light_draw_lights,         FE_SYSTEM_TICK);
    fe_ecs_register_system(fe_client_system_fog_draw,                  FE_SYSTEM_TICK);
    fe_ecs_register_system(fe_client_system_sprite2d_draw,             FE_SYSTEM_TICK);
    fe_ecs_register_system(fe_client_system_text2d_draw,               FE_SYSTEM_TICK);

    fe_ecs_register_system(cl_system_input_manager_tick,              FE_SYSTEM_TICK);
    fe_ecs_register_system(cl_system_game_manager_tick,               FE_SYSTEM_TICK);
    fe_ecs_register_system(cl_system_graphics_tick_texture_scrolling, FE_SYSTEM_TICK);

    fe_ecs_register_system(cl_system_fps_camera_tick_post, FE_SYSTEM_TICK);

}

static void
register_systems(void)
{
    register_fixed_tick_systems();
    register_tick_systems();
}

void 
cl_ecs_setup(void)
{
    register_components();
    register_queries();
    register_systems();

    cl_debug_manager_setup(); 
    cl_game_manager_setup(); 
    cl_input_manager_setup(); 
}

void 
cl_ecs_cleanup(void)
{
    cl_debug_manager_cleanup(); 
    cl_game_manager_cleanup(); 
    cl_input_manager_cleanup(); 
}
