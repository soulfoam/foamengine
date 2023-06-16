#pragma once

#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/return_code.h>
#include <foam/client/gfx/gl.h>

#define FE_MAX_TITLE_LEN 1024

typedef void (*fe_window_resize_fn)(const fe_vec2_t *size);

typedef enum
{
    FE_SCREEN_MODE_WINDOWED,
    FE_SCREEN_MODE_BORDERLESS,
    FE_SCREEN_MODE_FULLSCREEN,
    FE_SCREEN_MODE_FULLSCREEN_DESKTOP,
} FE_SCREEN_MODE;

FE_RETURN_CODE   fe_window_setup(const char *title, const fe_vec2_t *window_size, FE_SCREEN_MODE screen_mode);
void             fe_window_cleanup(void);
SDL_Window*      fe_window_get_sdl_window(void);
const fe_vec2_t* fe_window_get_display_resolution(void);
const fe_vec2_t* fe_window_get_size(void);
void             fe_window_set_size(const fe_vec2_t *size);
const char*      fe_window_get_title(void);
void             fe_window_set_title(const char *title);
bool             fe_window_get_vsync(void);
void             fe_window_set_vsync(bool on);
FE_SCREEN_MODE   fe_window_get_screen_mode(void);
void             fe_window_set_screen_mode(FE_SCREEN_MODE screen_mode);
bool             fe_window_is_grabbed(void);
void             fe_window_set_grab(bool grab);
void             fe_window_set_resize_callback(fe_window_resize_fn resize_fn);
void             fe_window_process_sdl_event(SDL_Event *e);


