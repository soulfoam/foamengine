#pragma once

#include <foam/core/ext/sugar.h>
#include <foam/core/return_code.h>

#define FE_DT fe_core_get_delta_time()
#define FE_FT fe_core_get_frame_time()
#define FE_TIME fe_core_get_time()

typedef void (*fe_core_tick_fn)(void);
typedef void (*fe_core_fixed_tick_fn)(void);

FE_RETURN_CODE fe_core_setup(u32 tick_rate, u32 component_count, u32 query_count);
void fe_core_cleanup(void);
void fe_core_start(fe_core_tick_fn tick, fe_core_fixed_tick_fn fixed_tick);
void fe_core_quit(void);
void fe_core_set_tick_rate(f64 tick_rate);
f64  fe_core_get_tick_rate(void);
f64  fe_core_get_delta_time(void);
f64  fe_core_get_frame_time(void);
f64  fe_core_get_time(void);
void fe_core_set_units_per_meter(f32 upm);
f32  fe_core_get_units_per_meter(void);
