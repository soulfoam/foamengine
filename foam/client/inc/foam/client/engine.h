#pragma once

#include <foam/core/core.h>
#include <foam/client/input.h>
#include <foam/client/gfx/renderer.h>
#include <foam/client/gfx/window.h>
#include <foam/core/config/config.h>
#include <foam/core/ext/logger.h>
#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/client/ecs/ecs_info.h>
#include <foam/core/return_code.h>
#include <foam/client/resource.h>

typedef void (*fe_client_tick_fn)(void);
typedef void (*fe_client_fixed_tick_fn)(void);

FE_RETURN_CODE fe_engine_setup(const fe_vec2_t *window_size, FE_SCREEN_MODE screen_mode,
                               const fe_vec2_t *render_resolution,
                               u32 tick_rate,
                               u32 component_count,
                               u32 query_count,
                               u32 shader_count);
void fe_engine_cleanup(void);
void fe_engine_start(fe_client_tick_fn tick_fn, fe_client_fixed_tick_fn fixed_tick_fn);
f32  fe_engine_get_render_time(void);
u32  fe_engine_get_fps(void);
