#include <foam/client/ecs/ecs_info.h>

static void
register_components(void)
{
    fe_ecs_register_component(FE_COMPONENT_CAMERA,        sizeof(fe_camera_t),        NULL);
    fe_ecs_register_component(FE_COMPONENT_MESH_INSTANCE, sizeof(fe_mesh_instance_t), fe_mesh_instance_kill);
    fe_ecs_register_component(FE_COMPONENT_FOG,           sizeof(fe_fog_t),           NULL);
    fe_ecs_register_component(FE_COMPONENT_LIGHT,         sizeof(fe_light_t),         fe_light_kill);
    fe_ecs_register_component(FE_COMPONENT_SKYBOX,        sizeof(fe_skybox_t),        fe_skybox_kill);
    fe_ecs_register_component(FE_COMPONENT_SPRITE3D,      sizeof(fe_sprite3d_t),      fe_sprite3d_kill);


    fe_ecs_register_component(FE_COMPONENT_SPRITE2D,      sizeof(fe_sprite2d_t),      NULL);
    fe_ecs_register_component(FE_COMPONENT_TEXT2D,        sizeof(fe_text2d_t),        fe_text2d_kill);
    fe_ecs_register_component(FE_COMPONENT_BUTTON2D,      sizeof(fe_button2d_t),      NULL);
}

static void
register_queries(void)
{
    fe_ecs_register_query(FE_QUERY_CAMERA,         FE_COMPONENT_CAMERA,        FE_COMPONENT_TRANSFORM);
    fe_ecs_register_query(FE_QUERY_MESH_INSTANCES, FE_COMPONENT_MESH_INSTANCE, FE_COMPONENT_TRANSFORM);
    fe_ecs_register_query(FE_QUERY_FOG,            FE_COMPONENT_FOG);
    fe_ecs_register_query(FE_QUERY_LIGHTS,         FE_COMPONENT_LIGHT,         FE_COMPONENT_TRANSFORM);
    fe_ecs_register_query(FE_QUERY_SKYBOX,         FE_COMPONENT_SKYBOX);
    fe_ecs_register_query(FE_QUERY_SPRITE3D,       FE_COMPONENT_SPRITE3D,      FE_COMPONENT_TRANSFORM);

    fe_ecs_register_query(FE_QUERY_TEXT2D,   FE_COMPONENT_TEXT2D,   FE_COMPONENT_TRANSFORM);
    fe_ecs_register_query(FE_QUERY_SPRITE2D, FE_COMPONENT_SPRITE2D, FE_COMPONENT_TRANSFORM);
    fe_ecs_register_query(FE_QUERY_BUTTON2D, FE_COMPONENT_BUTTON2D, FE_COMPONENT_TRANSFORM);
}

void 
fe_client_ecs_setup(void)
{
    register_components();
    register_queries();
}

void 
fe_client_ecs_cleanup(void)
{

}

