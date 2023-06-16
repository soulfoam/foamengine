#include <string.h>
#include <foam/client/input.h>
#include <foam/client/gfx/window.h>
#include <foam/core/ext/logger.h>
#include <foam/core/ext/vector.h>

#define clamp(v, min, max) (((v) > (max)) ? (max) : (((v) < (min)) ? (min) : (v)))

typedef struct
{    
    fe_vec2_t mouse_pos;
    fe_vec2_t mouse_motion;
    fe_vec2_t mouse_scroll;
    
    bool mouse_locked;
    bool mouse_locked_to_window; 

    fe_vec(fe_input_key_down_fn)          key_down_fn;
    fe_vec(fe_input_key_up_fn)            key_up_fn;
    fe_vec(fe_input_mouse_button_down_fn) mouse_button_down_fn;
    fe_vec(fe_input_mouse_button_up_fn)   mouse_button_up_fn;
    fe_vec(fe_input_mouse_move_fn)        mouse_move_fn;
    fe_vec(fe_input_mouse_scroll_fn)      mouse_scroll_fn;

    fe_vec(FE_KEY) key_presses;
} fe_input_t;

static fe_input_t input;

void
fe_input_setup(void)
{
    input.key_down_fn          = NULL;
    input.key_up_fn            = NULL;
    input.mouse_button_down_fn = NULL;
    input.mouse_button_up_fn   = NULL;
    input.mouse_move_fn        = NULL;
    input.mouse_scroll_fn      = NULL;
    input.key_presses          = FE_VEC_EMPTY;
    fe_input_reset();
}

void
fe_input_cleanup(void)
{
    fe_vec_kill(input.key_presses, NULL);
    fe_vec_kill(input.key_down_fn, NULL);
    fe_vec_kill(input.key_up_fn, NULL);
    fe_vec_kill(input.mouse_button_down_fn, NULL);
    fe_vec_kill(input.mouse_button_up_fn, NULL);
    fe_vec_kill(input.mouse_move_fn, NULL);
    fe_vec_kill(input.mouse_scroll_fn, NULL);
}

void
fe_input_reset(void)
{
    fe_vec2_zero(&input.mouse_pos);
    fe_vec2_zero(&input.mouse_motion);
    fe_vec2_zero(&input.mouse_scroll);
    input.mouse_locked = false;
    fe_vec_clear(input.key_presses, NULL);
}

void
fe_input_process_sdl_event(SDL_Event *e)
{
    switch (e->type)
    {
        case SDL_KEYDOWN:
            fe_vec_for_each(input.key_down_fn, kd) (*kd)((FE_KEY)e->key.keysym.scancode);
        break;

        case SDL_KEYUP:
            fe_vec_for_each(input.key_up_fn, ku) (*ku)((FE_KEY)e->key.keysym.scancode);
            fe_vec_add(input.key_presses, (FE_KEY)e->key.keysym.scancode);
        break;

        case SDL_MOUSEMOTION:
            fe_vec_for_each(input.mouse_move_fn, mm) (*mm)(&fe_vec2(e->motion.xrel, e->motion.yrel));
            input.mouse_motion.x = e->motion.xrel;
            input.mouse_motion.y = e->motion.yrel;
        break;

        case SDL_MOUSEWHEEL:
            fe_vec_for_each(input.mouse_scroll_fn, ms) (*ms)(&fe_vec2(e->wheel.x, e->wheel.y));
            input.mouse_scroll.x = -e->wheel.x;
            input.mouse_scroll.y = -e->wheel.y;
        break;

        case SDL_MOUSEBUTTONDOWN:
            fe_vec_for_each(input.mouse_button_down_fn, mu) (*mu)(e->button.button);
        break;

        case SDL_MOUSEBUTTONUP:
            fe_vec_for_each(input.mouse_button_up_fn, mu) (*mu)(e->button.button);
        break;

    }
}

void
fe_input_lock_mouse(bool lock)
{
    SDL_SetRelativeMouseMode(lock);
    input.mouse_locked = lock;
}

void
fe_input_lock_mouse_to_window(bool lock)
{
    SDL_SetWindowGrab(fe_window_get_sdl_window(), lock);
    input.mouse_locked_to_window = lock;
}

fe_vec2_t 
fe_input_get_mouse_position(void)
{
    i32 x, y;
    if (SDL_GetRelativeMouseMode()) 
    {
        SDL_GetRelativeMouseState(&x, &y);
    }
    else 
    {
        SDL_GetMouseState(&x, &y);
    }

    return fe_vec2(x, y);
}

fe_vec2_t
fe_input_get_mouse_motion(void)
{
    fe_vec2_t motion = input.mouse_motion;
    fe_vec2_norm(&motion);
    return motion;
}

fe_vec3_t
fe_input_get_mouse_motion_rotation(void)
{
    return FE_VEC3_ZERO;
}

bool
fe_input_did_mouse_scroll(void)
{
    return !fe_vec2_eq(&input.mouse_scroll, &fe_vec2(0, 0));
}

bool
fe_input_did_mouse_move(void)
{
    return !fe_vec2_eq(&input.mouse_motion, &fe_vec2(0, 0));
}

fe_vec2_t
fe_input_get_mouse_scroll(void)
{
    return input.mouse_scroll;
}

bool
fe_input_is_key_down(FE_KEY key)
{
    const u8 *state = SDL_GetKeyboardState(NULL);
    return state[key];
}

bool
fe_input_is_key_pressed(FE_KEY key)
{
    fe_vec_for_each(input.key_presses, k)
    {
        if (key == *k) return true;
    }

    return false;
}

bool
fe_input_is_mouse_button_down(u8 button)
{
   if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button)) return true;
   return false;
}

void 
fe_input_add_key_down_callback(fe_input_key_down_fn cb)
{
    fe_vec_add(input.key_down_fn, cb);
}

void 
fe_input_add_key_up_callback(fe_input_key_up_fn cb)
{
    fe_vec_add(input.key_up_fn, cb);
}

void 
fe_input_add_mouse_button_down_callback(fe_input_mouse_button_down_fn cb)
{
    fe_vec_add(input.mouse_button_down_fn, cb);
}

void 
fe_input_add_mouse_button_up_callback(fe_input_mouse_button_up_fn cb)
{
    fe_vec_add(input.mouse_button_up_fn, cb);
}

void 
fe_input_add_mouse_move_callback(fe_input_mouse_move_fn cb)
{
    fe_vec_add(input.mouse_move_fn, cb);
}

void 
fe_input_add_mouse_scroll_callback(fe_input_mouse_scroll_fn cb)
{
    fe_vec_add(input.mouse_scroll_fn, cb);
}

void 
fe_input_del_key_down_callback(fe_input_key_down_fn cb)
{
    fe_vec_for_each_index(input.key_down_fn, fn, i)
    {
        if ((*fn) == cb) fe_vec_del(input.key_down_fn, i, NULL);
    }
}

void 
fe_input_del_key_up_callback(fe_input_key_up_fn cb)
{
    fe_vec_for_each_index(input.key_up_fn, fn, i)
    {
        if ((*fn) == cb) fe_vec_del(input.key_up_fn, i, NULL);
    }
}

void 
fe_input_del_mouse_button_down_callback(fe_input_mouse_button_down_fn cb)
{
    fe_vec_for_each_index(input.mouse_button_down_fn, fn, i)
    {
        if ((*fn) == cb) fe_vec_del(input.mouse_button_down_fn, i, NULL);
    }
}

void 
fe_input_del_mouse_button_up_callback(fe_input_mouse_button_up_fn cb)
{
    fe_vec_for_each_index(input.mouse_button_up_fn, fn, i)
    {
        if ((*fn) == cb) fe_vec_del(input.mouse_button_up_fn, i, NULL);
    }
}

void 
fe_input_del_mouse_move_callback(fe_input_mouse_move_fn cb)
{
    fe_vec_for_each_index(input.mouse_move_fn, fn, i)
    {
        if ((*fn) == cb) fe_vec_del(input.mouse_move_fn, i, NULL);
    }
}

void 
fe_input_del_mouse_scroll_callback(fe_input_mouse_scroll_fn cb)
{
    fe_vec_for_each_index(input.mouse_scroll_fn, fn, i)
    {
        if ((*fn) == cb) fe_vec_del(input.mouse_scroll_fn, i, NULL);
    }
}

fe_ray_t
fe_ray_from_mouse(const fe_vec3_t *orig, const fe_mat4x4_t *view, const fe_mat4x4_t *proj)
{
    fe_vec2_t mouse_pos = fe_input_get_mouse_position();
    const fe_vec2_t *win_size = fe_window_get_size();
    fe_vec3_t ray_nds = fe_vec3
    (
        (2.0f * mouse_pos.x) / win_size->x - 1.0f,
        1.0f - (2.0f * mouse_pos.y) / win_size->y,
        1.0f
    );

    fe_vec4_t ray_clip = fe_vec4
    (
        ray_nds.x,
        1.0f,
        ray_nds.y,
        1.0f
    );

    fe_mat4x4_t inv_pm;
    fe_mat4x4_invert_to(&inv_pm, proj);

    fe_vec4_t ray_eye = fe_mat4x4_mul_vec4(&inv_pm, &ray_clip);
    ray_eye.y = 1.0f;
    ray_eye.w = 0.0f;

    fe_mat4x4_t inv_vm;
    fe_mat4x4_invert_to(&inv_vm, view);

    fe_vec4_t ray_wor = fe_mat4x4_mul_vec4(&inv_vm, &ray_eye);
    fe_vec4_norm(&ray_wor);
    
    return fe_ray(*orig, fe_vec3(ray_wor.x, ray_wor.y, ray_wor.z));
}

