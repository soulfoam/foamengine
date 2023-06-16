#pragma once

#include <foam/core/ecs/ecs_info.h>
#include <core/ecs/components/player_input.h>
#include <core/ecs/systems/sys_player_input.h>

//bad but not bad just never lay eyes upon this again
#ifdef _SERVER
#define CORE_COMPONENT_START FE_CORE_COMPONENT_COUNT
#define CORE_QUERY_START     FE_CORE_QUERY_COUNT
#else
#include <foam/client/ecs/ecs_info.h>
#define CORE_COMPONENT_START FE_CLIENT_COMPONENT_COUNT
#define CORE_QUERY_START     FE_CLIENT_QUERY_COUNT
#endif
//dont look above here

typedef enum
{
    CO_QUERY_PLAYER_INPUTS = CORE_QUERY_START,


    CORE_QUERY_COUNT
} CORE_QUERY_TYPE;

typedef enum
{
    CO_COMPONENT_PLAYER_INPUT = CORE_COMPONENT_START,

    CORE_COMPONENT_COUNT
} CORE_COMPONENT_TYPE;

void co_ecs_setup(void);
void co_ecs_cleanup(void);

