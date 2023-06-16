#pragma once

#include <foam/client/ecs/ecs_info.h>

#include <client/ecs/components/camera.h>
#include <client/ecs/components/editable.h>
#include <client/ecs/components/manager.h>
#include <client/ecs/components/input_manager.h>

#include <client/ecs/systems/sys_camera.h>
#include <client/ecs/systems/sys_manager.h>
#include <client/ecs/systems/sys_input_manager.h>
#include <client/ecs/systems/sys_editable.h>


typedef enum
{
    EDITOR_QUERY_EDITABLES = FE_CLIENT_QUERY_COUNT,

    EDITOR_QUERY_COUNT
} EDITOR_QUERY_TYPE;

typedef enum
{
    EDITOR_COMPONENT_CAMERA = FE_CLIENT_COMPONENT_COUNT,
    EDITOR_COMPONENT_EDITABLE,

    EDITOR_COMPONENT_COUNT
} EDITOR_COMPONENT_TYPE;

void editor_ecs_setup(void);
void editor_ecs_cleanup(void);
