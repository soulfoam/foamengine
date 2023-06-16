#pragma once

#include <foam/core/ecs/ecs.h>
#include <foam/core/ecs/systems/sys_transform.h>
#include <foam/core/ecs/systems/sys_transform_view.h>
#include <foam/core/ecs/systems/sys_rigidbody.h>
#include <foam/core/ecs/systems/sys_skeleton.h>
#include <foam/core/ecs/systems/sys_collision.h>
#include <foam/core/ecs/components/transform.h>
#include <foam/core/ecs/components/transform_view.h>
#include <foam/core/ecs/components/skeleton.h>
#include <foam/core/ecs/components/bone.h>
#include <foam/core/ecs/components/rigidbody.h>
#include <foam/core/ecs/components/mesh_collider.h>
#include <foam/core/ecs/components/simple_collider.h>


typedef enum
{
    FE_COMPONENT_TRANSFORM,
    FE_COMPONENT_TRANSFORM_VIEW,
    FE_COMPONENT_SKELETON,
    FE_COMPONENT_BONE,
    FE_COMPONENT_RIGIDBODY,
    FE_COMPONENT_MESH_COLLIDER,
    FE_COMPONENT_SIMPLE_COLLIDER,


    FE_CORE_COMPONENT_COUNT
} FE_CORE_COMPONENT_TYPE;

typedef enum
{
    FE_QUERY_TRANSFORMS,
    FE_QUERY_TRANSFORM_VIEWS,
    FE_QUERY_SKELETONS,
    FE_QUERY_BONES,
    FE_QUERY_RIGIDBODIES,
    FE_QUERY_MESH_COLLIDERS,
    FE_QUERY_SIMPLE_COLLIDERS,

    FE_CORE_QUERY_COUNT
} FE_CORE_QUERY;

void fe_core_ecs_setup(void);
void fe_core_ecs_cleanup(void);
