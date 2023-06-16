#include <foam/core/ext/timer.h>

#include <foam/core/core.h>
#include <foam/core/ecs/ecs.h>
#include <foam/core/network/client.h>
#include <foam/core/physics/physics.h>
#include <foam/client/ecs/ecs_info.h>
#include <foam/client/ecs/ecs_config.h>
#include <foam/client/engine.h>

typedef struct
{    
    u32 fps;

    f32 display_timer;
    u32 display_ticks;
    u32 display_frames;

    fe_client_tick_fn       tick_fn;
    fe_client_fixed_tick_fn fixed_tick_fn;

} fe_engine_t;

static fe_engine_t eng;

static void
events(void)
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        switch (e.type)
        {
            case SDL_QUIT:
                fe_core_quit();
            break;
        }
        
        fe_window_process_sdl_event(&e);
        fe_input_process_sdl_event(&e);
    }
}


static void
tick(void)
{
    events();
    fe_client_process();
    if (eng.tick_fn) eng.tick_fn();

    fe_input_reset();

	if (!fe_window_get_vsync()) SDL_Delay(1);
}

static void
fixed_tick(void)
{
    fe_client_fixed_tick();
    if (eng.fixed_tick_fn) eng.fixed_tick_fn();
}

FE_RETURN_CODE 
fe_engine_setup(const fe_vec2_t *window_size, FE_SCREEN_MODE screen_mode,
                const fe_vec2_t *render_resolution, 
                u32 tick_rate,
                u32 component_count,
                u32 query_count,
                u32 shader_count)
{
    eng.fps            = 0;

    eng.display_timer  = 0;
    eng.display_ticks  = 0;
    eng.display_frames = 0;

    eng.tick_fn       = NULL;
    eng.fixed_tick_fn = NULL;

    fe_core_setup(tick_rate, component_count, query_count);

    fe_client_resource_setup();
    fe_log_info("Client Resource initialized successfully");

    fe_client_ecs_setup();
    fe_client_register_config_callbacks();
    fe_log_info("ECS Client initialized successfully");
    
    fe_client_setup();
    fe_log_info("Network client initialized successfully");

    if (fe_window_setup("Foam Engine", window_size, screen_mode) == FE_OK)
    fe_log_info("Window initialized successfully");

    if (fe_renderer_setup(render_resolution, shader_count) == FE_OK)
    fe_log_info("Renderer initialized successfully");

    fe_input_setup();
    fe_log_info("Input initialized successfully");

    fe_log_info("Engine setup successfuly");

    return FE_OK;
}

void 
fe_engine_cleanup(void)
{
    fe_core_cleanup();
    fe_client_resource_cleanup();
    fe_client_ecs_cleanup();
    fe_client_cleanup();
    fe_window_cleanup();
    fe_renderer_cleanup();
    fe_input_cleanup();
}

void
fe_engine_start(fe_client_tick_fn tick_fn, fe_client_fixed_tick_fn fixed_tick_fn)
{
    eng.tick_fn       = tick_fn;
    eng.fixed_tick_fn = fixed_tick_fn;

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(loop, (void*)win, 0, 1);
#else
    fe_core_start(tick, fixed_tick);
#endif
}

u32 
fe_engine_get_fps(void)
{
    return eng.fps;
}


