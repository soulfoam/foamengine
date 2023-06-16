#pragma once

#include <client/shader.h>
#include <client/ecs/ecs_info.h>
#include <client/cl_net.h>
#include <foam/foam_core.h>
#include <core/core.h>

void cl_setup(void);
void cl_cleanup(void);
void cl_tick(void);
void cl_fixed_tick(void);
