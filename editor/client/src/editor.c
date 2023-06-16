#include <client/editor.h>

#define TICK_RATE 30

static void
window_resize(const fe_vec2_t *size)
{
    //fe_renderer_set_resolution(fe_window_get_size());
}

void
editor_setup(void)
{
    fe_engine_setup(&fe_vec2(3440/2, 1440/2), FE_SCREEN_MODE_WINDOWED, 
                    &fe_vec2(3440/2, 1440/2),
                    TICK_RATE,
                    EDITOR_COMPONENT_COUNT,
                    EDITOR_QUERY_COUNT,
                    SHADER_COUNT);

    fe_renderer_set_clear_color(&fe_color_rgb(0, 0, 0));

    fe_renderer_register_shader(FE_SHADER_INSTANCING, "res/shaders/def.fesh");
    fe_renderer_register_shader(FE_SHADER_UI,         "res/shaders/ui.fesh");

    editor_ecs_setup();

    ed_manager_add_entity_from_file("res/configs/map.feco");
    ed_manager_add_entity_from_file("res/configs/knight.feco");
    fe_window_set_resize_callback(window_resize);
}

void
editor_cleanup(void)
{
    editor_ecs_cleanup();
    fe_engine_cleanup();
}

void
editor_tick(void)
{
    fe_renderer_start_draw();
    fe_ecs_run_systems(FE_SYSTEM_TICK);
    fe_renderer_end_draw();
    fe_renderer_render();

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        fe_log("GL Error [%d]", err);
    }

	//eng.display_frames++;
    //eng.display_timer += FE_DT;
    //
	//if (eng.display_timer >= 1000)
	//{
    //    char title[FE_MAX_TITLE_LEN+256];
    //    memset(title, '\0', FE_MAX_TITLE_LEN+256);
    //    sprintf(title, "%s [FPS: %u | TPS: %u | DT: %f | RT: %f]", fe_window_get_title(), 
    //            eng.display_frames, eng.display_ticks, FE_DT, FE_FT);
    //    SDL_SetWindowTitle(fe_window_get_sdl_window(), title);
    //    fe_log("%s", title);
	//	eng.fps = eng.display_frames;
    //    eng.display_timer  = 0;
	//	eng.display_frames = 0;
	//	eng.display_ticks  = 0;
	//}

}

void
editor_fixed_tick(void)
{
    fe_ecs_run_systems(FE_SYSTEM_FIXED_TICK);
}

