#pragma once

#include <foam/foam_client.h>
#include <client/shader.h>
#include <client/ecs/ecs_info.h>
#include <client/cl_net.h>

void editor_setup(void);
void editor_cleanup(void);
void editor_tick(void);
void editor_fixed_tick(void);
