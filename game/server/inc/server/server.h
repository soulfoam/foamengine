#pragma once

#include <foam/foam_core.h>
#include <server/ecs/ecs_info.h>

void sv_setup(u16 port);
void sv_cleanup(void);
void sv_start(void);
