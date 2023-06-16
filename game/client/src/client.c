#include <client/client.h>

#define TICK_RATE 25

static void
window_resize(const fe_vec2_t *size)
{
    //fe_renderer_set_resolution(fe_window_get_size());
}

void
cl_setup(void)
{
    fe_engine_setup(&fe_vec2(3440/2, 1440/2), FE_SCREEN_MODE_WINDOWED, 
                    &fe_vec2(3440, 1440),
                    TICK_RATE,
                    CLIENT_COMPONENT_COUNT,
                    CLIENT_QUERY_COUNT,
                    SHADER_COUNT);

    fe_client_set_callbacks(cl_net_on_connect, cl_net_on_disconnect, cl_net_on_read);
    fe_client_connect("127.0.0.1", 1337);
    fe_writer_t writer;
    fe_writer_make(&writer);
    fe_io_write_string(&writer, "sup brah");
    fe_io_write_string(&writer, "sjdj");
    fe_client_write(writer.buffer, writer.size, false);
    fe_writer_kill(&writer);

    fe_renderer_set_clear_color(&fe_color_rgb(0, 0, 0));
    fe_input_lock_mouse(true);

    fe_renderer_register_shader(FE_SHADER_INSTANCING, "res/shaders/def.fesh");
    fe_renderer_register_shader(FE_SHADER_SKYBOX,     "res/shaders/skybox.fesh");
    fe_renderer_register_shader(FE_SHADER_UI,         "res/shaders/ui.fesh");

    co_setup();
    cl_ecs_setup();

    fe_window_set_resize_callback(window_resize);
}

void
cl_cleanup(void)
{
    co_cleanup();
    cl_ecs_cleanup();
    fe_engine_cleanup();
}

void
cl_tick(void)
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
cl_fixed_tick(void)
{
    fe_ecs_run_systems(FE_SYSTEM_FIXED_TICK);
}

