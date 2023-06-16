#include <foam/core/ecs/ecs_info.h>

static void
register_components(void)
{
    fe_ecs_register_component(FE_COMPONENT_TRANSFORM,       sizeof(fe_transform_t),       NULL);
    fe_ecs_register_component(FE_COMPONENT_TRANSFORM_VIEW,  sizeof(fe_transform_view_t),       NULL);
    fe_ecs_register_component(FE_COMPONENT_SKELETON,        sizeof(fe_skeleton_t),        fe_skeleton_kill);
    fe_ecs_register_component(FE_COMPONENT_BONE,            sizeof(fe_bone_t),            NULL);
    fe_ecs_register_component(FE_COMPONENT_RIGIDBODY,       sizeof(fe_rigidbody_t),       NULL);
    fe_ecs_register_component(FE_COMPONENT_MESH_COLLIDER,   sizeof(fe_mesh_collider_t),   fe_mesh_collider_kill);
    fe_ecs_register_component(FE_COMPONENT_SIMPLE_COLLIDER, sizeof(fe_simple_collider_t), fe_simple_collider_kill);
}

static void
register_queries(void)
{
    fe_ecs_register_query(FE_QUERY_TRANSFORMS,       FE_COMPONENT_TRANSFORM);
    fe_ecs_register_query(FE_QUERY_TRANSFORM_VIEWS,  FE_COMPONENT_TRANSFORM, FE_COMPONENT_TRANSFORM_VIEW);
    fe_ecs_register_query(FE_QUERY_SKELETONS,        FE_COMPONENT_SKELETON);
    fe_ecs_register_query(FE_QUERY_BONES,            FE_COMPONENT_BONE,      FE_COMPONENT_TRANSFORM);
    fe_ecs_register_query(FE_QUERY_RIGIDBODIES,      FE_COMPONENT_TRANSFORM, FE_COMPONENT_RIGIDBODY);
    fe_ecs_register_query(FE_QUERY_MESH_COLLIDERS,   FE_COMPONENT_TRANSFORM, FE_COMPONENT_MESH_COLLIDER);
    fe_ecs_register_query(FE_QUERY_SIMPLE_COLLIDERS, FE_COMPONENT_TRANSFORM, FE_COMPONENT_SIMPLE_COLLIDER);
}

void 
fe_core_ecs_setup(void)
{
    register_components();
    register_queries();
}

void 
fe_core_ecs_cleanup(void)
{

}
