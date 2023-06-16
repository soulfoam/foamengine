#pragma once

#include <core/ecs/ecs_info.h>
#include <foam/client/ecs/ecs_info.h>

#include <client/ecs/components/debug_manager.h>
#include <client/ecs/components/fps_camera.h>
#include <client/ecs/components/game_manager.h>
#include <client/ecs/components/input_manager.h>

#include <client/ecs/systems/sys_debug_manager.h>
#include <client/ecs/systems/sys_graphics.h>
#include <client/ecs/systems/sys_fps_camera.h>
#include <client/ecs/systems/sys_game_manager.h>
#include <client/ecs/systems/sys_input_manager.h>


typedef enum
{
    CL_QUERY = FE_CLIENT_QUERY_COUNT,

    CLIENT_QUERY_COUNT
} CLIENT_QUERY_TYPE;

typedef enum
{
    CL_COMPONENT_FPS_CAMERA = CORE_COMPONENT_COUNT,

    CLIENT_COMPONENT_COUNT
} CLIENT_COMPONENT_TYPE;

void cl_ecs_setup(void);
void cl_ecs_cleanup(void);
