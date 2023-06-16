#define STB_IMAGE_IMPLEMENTATION
#include <foam/core/data/stb_image.h>

#include <foam/core/ext/timer.h>
#include <foam/core/core.h>
#include <foam/core/data/resource.h>
#include <foam/core/data/cache.h>
#include <foam/core/ecs/ecs.h>
#include <foam/core/ecs/ecs_info.h>
#include <foam/core/network/client.h>
#include <foam/core/physics/physics.h>
#include <foam/core/ecs/ecs_config.h>

typedef struct
{    
    bool quit;
    f64  tick_rate;
    u64  now_time;
    u64  last_time;
    f64  accumulator;
    f64  delta_time;
    f64  frame_time;

    f32 units_per_meter;

    f64 time;

} fe_core_t;

static fe_core_t core;

FE_RETURN_CODE 
fe_core_setup(u32 tick_rate, u32 component_count, u32 query_count)
{
    fe_core_set_tick_rate(tick_rate);

    core.quit            = false;
    core.now_time        = fe_time_ns();
    core.delta_time      = (f64)((core.now_time - core.last_time)*1000 / (f64)1000000000);
    core.last_time       = core.now_time;
    core.accumulator     = 0;
    core.frame_time      = 0;
    core.units_per_meter = 1;
    core.time            = 0.0f;

    fe_ecs_setup(component_count, query_count);
    fe_core_ecs_setup();
    fe_core_ecs_register_config_callbacks();
    fe_log_info("ECS Core initialized successfully");

    if (fe_core_cache_setup(true) == FE_OK)
    fe_log_info("Cache initialized successfully");

    fe_core_resource_setup();
    fe_log_info("Core Resource initialized successfully");

    fe_physics_setup();
    fe_log_info("Physics initialized successfully");

    fe_log_info("Core setup successfuly");

    return FE_OK;
}

void
fe_core_cleanup(void)
{
    fe_ecs_cleanup();
    fe_core_ecs_cleanup();
    fe_core_cache_cleanup();
    fe_core_resource_cleanup();
    fe_physics_cleanup();
}

void
fe_core_start(fe_core_tick_fn tick, fe_core_fixed_tick_fn fixed_tick)
{
    while (!core.quit)
    {
        core.now_time   = fe_time_ns();
        core.delta_time = (f64)((core.now_time - core.last_time)*1000 / (f64)1000000000);
        core.last_time  = core.now_time;

        core.accumulator += core.delta_time;
        core.time += core.delta_time;

        while (core.accumulator >= core.tick_rate)
        {
            if (fixed_tick) fixed_tick();
            core.accumulator -= core.tick_rate;
        }

        core.frame_time = core.accumulator / core.tick_rate;

        if (tick) tick();
    }
}

void
fe_core_quit(void)
{
    core.quit = true;
}

void
fe_core_set_tick_rate(f64 tick_rate)
{
	core.tick_rate = (1000.0f / tick_rate);
}

f64
fe_core_get_tick_rate(void)
{
	return core.tick_rate;
}

f64
fe_core_get_delta_time(void)
{
    return core.delta_time;
}

f64
fe_core_get_frame_time(void)
{
    return core.frame_time;
}

f64
fe_core_get_time(void)
{
    return core.time;
}

void 
fe_core_set_units_per_meter(f32 upm)
{
    core.units_per_meter = upm;
}

f32  
fe_core_get_units_per_meter(void)
{
    return core.units_per_meter;
}

