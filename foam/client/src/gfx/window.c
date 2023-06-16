#ifdef _WIN32
#include <windows.h>
#define sleep(n) Sleep(n)
#else
#include <unistd.h>
#define sleep(n) usleep((n) * 1000)
#endif

#include <stdio.h>
#include <foam/core/ext/string.h>
#include <foam/core/data/stb_image.h>
#include <foam/core/ext/logger.h>
#include <foam/client/gfx/window.h>

#define RES_WIDTH  1280
#define RES_HEIGHT 720

typedef struct
{    
    char           title[FE_MAX_TITLE_LEN];
    FE_SCREEN_MODE screen_mode;
    bool           vsync;
    bool           grabbed;
    fe_vec2_t      size;
    fe_vec2_t      display_resolution;

    SDL_Window    *sdl_window;
    SDL_GLContext *sdl_context;

    fe_window_resize_fn resize_fn;
} fe_window_t;

static fe_window_t win;

static void
sdl_error(const char *message)
{
    fe_log_fatal("%s - %s", message, SDL_GetError());
    exit(1);
}

static void
window_set_size(void)
{
    i32 x, y;
    SDL_GetWindowSize(win.sdl_window, &x, &y);
    win.size = fe_vec2(x, y);
}

static void
window_set_display_resolution(void)
{
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    win.display_resolution.x = dm.w;
    win.display_resolution.y = dm.h;
}

FE_RETURN_CODE
fe_window_setup(const char *title, const fe_vec2_t *window_size, FE_SCREEN_MODE screen_mode)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) sdl_error("Failed to init SDL Video.");

    SDL_GL_LoadLibrary(NULL);

#ifndef FE_OPENGL_ES
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,    1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#else
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,    1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0); 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   24);

    win.screen_mode = screen_mode;

    u32 sdl_flags = 0;
    if (screen_mode == FE_SCREEN_MODE_FULLSCREEN)            
    {
        sdl_flags |= SDL_WINDOW_FULLSCREEN;
    }
    else if (screen_mode == FE_SCREEN_MODE_FULLSCREEN_DESKTOP)            
    {
        sdl_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    else if (screen_mode == FE_SCREEN_MODE_BORDERLESS)
    {
        sdl_flags |= SDL_WINDOW_BORDERLESS;
    }

    sdl_flags |= SDL_WINDOW_OPENGL;

    win.sdl_window = SDL_CreateWindow
                     (
                         title,
                         SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                         window_size->x, window_size->y,
                         sdl_flags
                     );

    if (win.sdl_window == NULL)
    {
        sdl_error("Failed to create SDL Window.");
        return FE_ERR;
    }

    win.sdl_context = SDL_GL_CreateContext(win.sdl_window);

    if (win.sdl_context == NULL) 
    {
        sdl_error("Failed to create OpenGL context.");
        return FE_ERR;
    }

    win.resize_fn = NULL;

#ifndef FE_OPENGL_ES
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    SDL_GL_SetSwapInterval(0);
#endif

    fe_window_set_title(title);
	fe_window_set_size(window_size);
    fe_window_set_grab(false);
    fe_window_set_vsync(false);


	fe_log_info("Vendor:   %s", glGetString(GL_VENDOR));
	fe_log_info("Renderer: %s", glGetString(GL_RENDERER));
	fe_log_info("Version:  %s", glGetString(GL_VERSION)); 
	fe_log_info("Size:     %ux%u", (u32)window_size->x, (u32)window_size->y); 
	fe_log_info("VSync:    %u", win.vsync); 

    return FE_OK;

}

void
fe_window_cleanup(void)
{
    SDL_GL_DeleteContext(win.sdl_context);
    SDL_DestroyWindow(win.sdl_window);
    SDL_Quit();
    //exit(0);
}

SDL_Window*
fe_window_get_sdl_window(void)
{
    return win.sdl_window;
}

const fe_vec2_t*
fe_window_get_size(void)
{
    window_set_size();
    return &win.size;
}

const fe_vec2_t*
fe_window_get_display_resolution(void)
{
    window_set_display_resolution();
    return &win.display_resolution;
}

void       
fe_window_set_size(const fe_vec2_t *size)
{
    win.size = *size;
	SDL_SetWindowSize(win.sdl_window, (i32)size->x, (i32)size->y);
}

const char*
fe_window_get_title(void)
{
    return win.title;
}

void
fe_window_set_title(const char *val)
{
    fe_string_copy(win.title, val, FE_MAX_TITLE_LEN);
	SDL_SetWindowTitle(win.sdl_window, win.title);
}

bool
fe_window_get_vsync(void)
{
    return win.vsync;
}

void
fe_window_set_vsync(bool on)
{
    win.vsync = on;

    if (on)
    {
        if (SDL_GL_SetSwapInterval(1) < 0)
        {
            fe_log_warn("Unable to set v-sync!");
        }
        SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    }
    else
    {

        SDL_GL_SetSwapInterval(0);
        SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
    }
}

FE_SCREEN_MODE 
fe_window_get_screen_mode(void)
{
    return win.screen_mode;
}

void             
fe_window_set_screen_mode(FE_SCREEN_MODE screen_mode)
{
    win.screen_mode = screen_mode;

    u32 sdl_flags = 0;

    if (screen_mode == FE_SCREEN_MODE_WINDOWED)                   
    {
        sdl_flags = 0;
    }
    else if (screen_mode == FE_SCREEN_MODE_FULLSCREEN)            
    {
        sdl_flags = SDL_WINDOW_FULLSCREEN;
    }
    else if (screen_mode == FE_SCREEN_MODE_BORDERLESS) 
    {
        sdl_flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

	SDL_SetWindowFullscreen(win.sdl_window, sdl_flags);
}

void
fe_window_set_grab(bool grab)
{
    win.grabbed = grab;
	SDL_SetWindowGrab(win.sdl_window, grab);
}

bool
fe_window_is_grabbed(void)
{
    return win.grabbed;
}

void
fe_window_set_resize_callback(fe_window_resize_fn resize_fn)
{
    win.resize_fn = resize_fn;
}

void
fe_window_process_sdl_event(SDL_Event *e)
{
    switch (e->type)
    {
        case SDL_WINDOWEVENT:
        if (e->window.event == SDL_WINDOWEVENT_RESIZED)
        {
            if (win.resize_fn) win.resize_fn(fe_window_get_size());
        }
        break;
    }
}
    
