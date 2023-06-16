#pragma once

#include <foam/foam_core.h>

typedef enum
{
    SV_QUERY = FE_CORE_QUERY_COUNT,

    SERVER_QUERY_COUNT
} SERVER_QUERY_TYPE;

typedef enum
{
    SV_COMPONENT = FE_CORE_COMPONENT_COUNT,

    SERVER_COMPONENT_COUNT
} SERVER_COMPONENT_TYPE;
