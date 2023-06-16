#pragma once

#include <foam/core/ecs/ecs_info.h>

#include <foam/client/ecs/systems/sys_camera.h>
#include <foam/client/ecs/systems/sys_light.h>
#include <foam/client/ecs/systems/sys_fog.h>
#include <foam/client/ecs/systems/sys_skybox.h>
#include <foam/client/ecs/systems/sys_mesh_instance.h>
#include <foam/client/ecs/systems/sys_debug.h>
#include <foam/client/ecs/systems/sys_sprite2d.h>
#include <foam/client/ecs/systems/sys_sprite3d.h>
#include <foam/client/ecs/systems/sys_text2d.h>
#include <foam/client/ecs/systems/sys_button2d.h>

#include <foam/client/ecs/components/camera.h>
#include <foam/client/ecs/components/mesh_instance.h>
#include <foam/client/ecs/components/fog.h>
#include <foam/client/ecs/components/light.h>
#include <foam/client/ecs/components/skybox.h>
#include <foam/client/ecs/components/text2d.h>
#include <foam/client/ecs/components/button2d.h>
#include <foam/client/ecs/components/sprite2d.h>
#include <foam/client/ecs/components/sprite3d.h>



typedef enum
{
    FE_COMPONENT_CAMERA = FE_CORE_COMPONENT_COUNT,
    FE_COMPONENT_MESH_INSTANCE,
    FE_COMPONENT_FOG,
    FE_COMPONENT_LIGHT,
    FE_COMPONENT_SKYBOX,
    FE_COMPONENT_SPRITE3D,


    FE_COMPONENT_SPRITE2D,
    FE_COMPONENT_TEXT2D,
    FE_COMPONENT_BUTTON2D,


    FE_CLIENT_COMPONENT_COUNT
} FE_CLIENT_COMPONENT_TYPE;

typedef enum
{
    FE_QUERY_CAMERA = FE_CORE_QUERY_COUNT,
    FE_QUERY_MESH_INSTANCES,
    FE_QUERY_FOG,
    FE_QUERY_LIGHTS,
    FE_QUERY_SKYBOX,
    FE_QUERY_SPRITE3D,


    FE_QUERY_SPRITE2D,
    FE_QUERY_TEXT2D,
    FE_QUERY_BUTTON2D,

    FE_CLIENT_QUERY_COUNT
} FE_CLIENT_QUERY_TYPE;

void fe_client_ecs_setup(void);
void fe_client_ecs_cleanup(void);
