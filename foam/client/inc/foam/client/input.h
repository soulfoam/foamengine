#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/math/math.h>
#include <foam/core/physics/ray.h>

typedef enum
{
    FE_KEY_UNKNOWN            = SDL_SCANCODE_UNKNOWN,

    FE_KEY_A                  = SDL_SCANCODE_A,
    FE_KEY_B                  = SDL_SCANCODE_B,
    FE_KEY_C                  = SDL_SCANCODE_C,
    FE_KEY_D                  = SDL_SCANCODE_D,
    FE_KEY_E                  = SDL_SCANCODE_E,
    FE_KEY_F                  = SDL_SCANCODE_F,
    FE_KEY_G                  = SDL_SCANCODE_G,
    FE_KEY_H                  = SDL_SCANCODE_H,
    FE_KEY_I                  = SDL_SCANCODE_I,
    FE_KEY_J                  = SDL_SCANCODE_J,
    FE_KEY_K                  = SDL_SCANCODE_K,
    FE_KEY_L                  = SDL_SCANCODE_L,
    FE_KEY_M                  = SDL_SCANCODE_M,
    FE_KEY_N                  = SDL_SCANCODE_N,
    FE_KEY_O                  = SDL_SCANCODE_O,
    FE_KEY_P                  = SDL_SCANCODE_P,
    FE_KEY_Q                  = SDL_SCANCODE_Q,
    FE_KEY_R                  = SDL_SCANCODE_R,
    FE_KEY_S                  = SDL_SCANCODE_S,
    FE_KEY_T                  = SDL_SCANCODE_T,
    FE_KEY_U                  = SDL_SCANCODE_U,
    FE_KEY_V                  = SDL_SCANCODE_V,
    FE_KEY_W                  = SDL_SCANCODE_W,
    FE_KEY_X                  = SDL_SCANCODE_X,
    FE_KEY_Y                  = SDL_SCANCODE_Y,
    FE_KEY_Z                  = SDL_SCANCODE_Z,

    FE_KEY_1                  = SDL_SCANCODE_1,
    FE_KEY_2                  = SDL_SCANCODE_2,
    FE_KEY_3                  = SDL_SCANCODE_3,
    FE_KEY_4                  = SDL_SCANCODE_4,
    FE_KEY_5                  = SDL_SCANCODE_5,
    FE_KEY_6                  = SDL_SCANCODE_6,
    FE_KEY_7                  = SDL_SCANCODE_7,
    FE_KEY_8                  = SDL_SCANCODE_8,
    FE_KEY_9                  = SDL_SCANCODE_9,
    FE_KEY_0                  = SDL_SCANCODE_0,

    FE_KEY_RETURN             = SDL_SCANCODE_RETURN,
    FE_KEY_ESCAPE             = SDL_SCANCODE_ESCAPE,
    FE_KEY_BACKSPACE          = SDL_SCANCODE_BACKSPACE,
    FE_KEY_TAB                = SDL_SCANCODE_TAB,
    FE_KEY_SPACE              = SDL_SCANCODE_SPACE,

    FE_KEY_MINUS              = SDL_SCANCODE_MINUS,
    FE_KEY_EQUALS             = SDL_SCANCODE_EQUALS,
    FE_KEY_LEFTBRACKET        = SDL_SCANCODE_LEFTBRACKET,
    FE_KEY_RIGHTBRACKET       = SDL_SCANCODE_RIGHTBRACKET,
    FE_KEY_BACKSLASH          = SDL_SCANCODE_BACKSLASH,
    FE_KEY_NONUSHASH          = SDL_SCANCODE_NONUSHASH,
    FE_KEY_SEMICOLON          = SDL_SCANCODE_SEMICOLON,
    FE_KEY_APOSTROPHE         = SDL_SCANCODE_APOSTROPHE,
    FE_KEY_GRAVE              = SDL_SCANCODE_GRAVE,
    FE_KEY_COMMA              = SDL_SCANCODE_COMMA,
    FE_KEY_PERIOD             = SDL_SCANCODE_PERIOD,
    FE_KEY_SLASH              = SDL_SCANCODE_SLASH,

    FE_KEY_CAPSLOCK           = SDL_SCANCODE_CAPSLOCK,

    FE_KEY_F1                 = SDL_SCANCODE_F1,
    FE_KEY_F2                 = SDL_SCANCODE_F2,
    FE_KEY_F3                 = SDL_SCANCODE_F3,
    FE_KEY_F4                 = SDL_SCANCODE_F4,
    FE_KEY_F5                 = SDL_SCANCODE_F5,
    FE_KEY_F6                 = SDL_SCANCODE_F6,
    FE_KEY_F7                 = SDL_SCANCODE_F7,
    FE_KEY_F8                 = SDL_SCANCODE_F8,
    FE_KEY_F9                 = SDL_SCANCODE_F9,
    FE_KEY_F10                = SDL_SCANCODE_F10,
    FE_KEY_F11                = SDL_SCANCODE_F11,
    FE_KEY_F12                = SDL_SCANCODE_F12,

    FE_KEY_PRINTSCREEN        = SDL_SCANCODE_PRINTSCREEN,
    FE_KEY_SCROLLLOCK         = SDL_SCANCODE_SCROLLLOCK,
    FE_KEY_PAUSE              = SDL_SCANCODE_PAUSE,
    FE_KEY_INSERT             = SDL_SCANCODE_INSERT,
    FE_KEY_HOME               = SDL_SCANCODE_HOME,
    FE_KEY_PAGEUP             = SDL_SCANCODE_PAGEUP,
    FE_KEY_DELETE             = SDL_SCANCODE_DELETE,
    FE_KEY_END                = SDL_SCANCODE_END,
    FE_KEY_PAGEDOWN           = SDL_SCANCODE_PAGEDOWN,
    FE_KEY_RIGHT              = SDL_SCANCODE_RIGHT,
    FE_KEY_LEFT               = SDL_SCANCODE_LEFT,
    FE_KEY_DOWN               = SDL_SCANCODE_DOWN,
    FE_KEY_UP                 = SDL_SCANCODE_UP,

    FE_KEY_NUMLOCKCLEAR       = SDL_SCANCODE_NUMLOCKCLEAR,
    FE_KEY_KP_DIVIDE          = SDL_SCANCODE_KP_DIVIDE,
    FE_KEY_KP_MULTIPLY        = SDL_SCANCODE_KP_MULTIPLY,
    FE_KEY_KP_MINUS           = SDL_SCANCODE_KP_MINUS,
    FE_KEY_KP_PLUS            = SDL_SCANCODE_KP_PLUS,
    FE_KEY_KP_ENTER           = SDL_SCANCODE_KP_ENTER,
    FE_KEY_KP_1               = SDL_SCANCODE_KP_1,
    FE_KEY_KP_2               = SDL_SCANCODE_KP_2,
    FE_KEY_KP_3               = SDL_SCANCODE_KP_3,
    FE_KEY_KP_4               = SDL_SCANCODE_KP_4,
    FE_KEY_KP_5               = SDL_SCANCODE_KP_5,
    FE_KEY_KP_6               = SDL_SCANCODE_KP_6,
    FE_KEY_KP_7               = SDL_SCANCODE_KP_7,
    FE_KEY_KP_8               = SDL_SCANCODE_KP_8,
    FE_KEY_KP_9               = SDL_SCANCODE_KP_9,
    FE_KEY_KP_0               = SDL_SCANCODE_KP_0,
    FE_KEY_KP_PERIOD          = SDL_SCANCODE_KP_PERIOD,

    FE_KEY_NONUSBACKSLASH     = SDL_SCANCODE_NONUSBACKSLASH,
    FE_KEY_APPLICATION        = SDL_SCANCODE_APPLICATION,
    FE_KEY_POWER              = SDL_SCANCODE_POWER,
    FE_KEY_KP_EQUALS          = SDL_SCANCODE_KP_EQUALS,
    FE_KEY_F13                = SDL_SCANCODE_F13,
    FE_KEY_F14                = SDL_SCANCODE_F14,
    FE_KEY_F15                = SDL_SCANCODE_F15,
    FE_KEY_F16                = SDL_SCANCODE_F16,
    FE_KEY_F17                = SDL_SCANCODE_F17,
    FE_KEY_F18                = SDL_SCANCODE_F18,
    FE_KEY_F19                = SDL_SCANCODE_F19,
    FE_KEY_F20                = SDL_SCANCODE_F20,
    FE_KEY_F21                = SDL_SCANCODE_F21,
    FE_KEY_F22                = SDL_SCANCODE_F22,
    FE_KEY_F23                = SDL_SCANCODE_F23,
    FE_KEY_F24                = SDL_SCANCODE_F24,
    FE_KEY_EXECUTE            = SDL_SCANCODE_EXECUTE,
    FE_KEY_HELP               = SDL_SCANCODE_HELP,
    FE_KEY_MENU               = SDL_SCANCODE_MENU,
    FE_KEY_SELECT             = SDL_SCANCODE_SELECT,
    FE_KEY_STOP               = SDL_SCANCODE_STOP,
    FE_KEY_AGAIN              = SDL_SCANCODE_AGAIN,
    FE_KEY_UNDO               = SDL_SCANCODE_UNDO,
    FE_KEY_CUT                = SDL_SCANCODE_CUT,
    FE_KEY_COPY               = SDL_SCANCODE_COPY,
    FE_KEY_PASTE              = SDL_SCANCODE_PASTE,
    FE_KEY_FIND               = SDL_SCANCODE_FIND,
    FE_KEY_MUTE               = SDL_SCANCODE_MUTE,
    FE_KEY_VOLUMEUP           = SDL_SCANCODE_VOLUMEUP,
    FE_KEY_VOLUMEDOWN         = SDL_SCANCODE_VOLUMEDOWN,
    FE_KEY_KP_COMMA           = SDL_SCANCODE_KP_COMMA,
    FE_KEY_KP_EQUALSAS400     = SDL_SCANCODE_KP_EQUALSAS400,

    FE_KEY_INTERNATIONAL1     = SDL_SCANCODE_INTERNATIONAL1,
    FE_KEY_INTERNATIONAL2     = SDL_SCANCODE_INTERNATIONAL2,
    FE_KEY_INTERNATIONAL3     = SDL_SCANCODE_INTERNATIONAL3,
    FE_KEY_INTERNATIONAL4     = SDL_SCANCODE_INTERNATIONAL4,
    FE_KEY_INTERNATIONAL5     = SDL_SCANCODE_INTERNATIONAL5,
    FE_KEY_INTERNATIONAL6     = SDL_SCANCODE_INTERNATIONAL6,
    FE_KEY_INTERNATIONAL7     = SDL_SCANCODE_INTERNATIONAL7,
    FE_KEY_INTERNATIONAL8     = SDL_SCANCODE_INTERNATIONAL8,
    FE_KEY_INTERNATIONAL9     = SDL_SCANCODE_INTERNATIONAL9,
    FE_KEY_LANG1              = SDL_SCANCODE_LANG1,
    FE_KEY_LANG2              = SDL_SCANCODE_LANG2,
    FE_KEY_LANG3              = SDL_SCANCODE_LANG3,
    FE_KEY_LANG4              = SDL_SCANCODE_LANG4,
    FE_KEY_LANG5              = SDL_SCANCODE_LANG5,
    FE_KEY_LANG6              = SDL_SCANCODE_LANG6,
    FE_KEY_LANG7              = SDL_SCANCODE_LANG7,
    FE_KEY_LANG8              = SDL_SCANCODE_LANG8,
    FE_KEY_LANG9              = SDL_SCANCODE_LANG9,

    FE_KEY_ALTERASE           = SDL_SCANCODE_ALTERASE,
    FE_KEY_SYSREQ             = SDL_SCANCODE_SYSREQ,
    FE_KEY_CANCEL             = SDL_SCANCODE_CANCEL,
    FE_KEY_CLEAR              = SDL_SCANCODE_CLEAR,
    FE_KEY_PRIOR              = SDL_SCANCODE_PRIOR,
    FE_KEY_RETURN2            = SDL_SCANCODE_RETURN2,
    FE_KEY_SEPARATOR          = SDL_SCANCODE_SEPARATOR,
    FE_KEY_OUT                = SDL_SCANCODE_OUT,
    FE_KEY_OPER               = SDL_SCANCODE_OPER,
    FE_KEY_CLEARAGAIN         = SDL_SCANCODE_CLEARAGAIN,
    FE_KEY_CRSEL              = SDL_SCANCODE_CRSEL,
    FE_KEY_EXSEL              = SDL_SCANCODE_EXSEL,

    FE_KEY_KP_00              = SDL_SCANCODE_KP_00,
    FE_KEY_KP_000             = SDL_SCANCODE_KP_000,
    FE_KEY_THOUSANDSSEPARATOR = SDL_SCANCODE_THOUSANDSSEPARATOR,
    FE_KEY_DECIMALSEPARATOR   = SDL_SCANCODE_DECIMALSEPARATOR,
    FE_KEY_CURRENCYUNIT       = SDL_SCANCODE_CURRENCYUNIT,
    FE_KEY_CURRENCYSUBUNIT    = SDL_SCANCODE_CURRENCYSUBUNIT,
    FE_KEY_KP_LEFTPAREN       = SDL_SCANCODE_KP_LEFTPAREN,
    FE_KEY_KP_RIGHTPAREN      = SDL_SCANCODE_KP_RIGHTPAREN,
    FE_KEY_KP_LEFTBRACE       = SDL_SCANCODE_KP_LEFTBRACE,
    FE_KEY_KP_RIGHTBRACE      = SDL_SCANCODE_KP_RIGHTBRACE,
    FE_KEY_KP_TAB             = SDL_SCANCODE_KP_TAB,
    FE_KEY_KP_BACKSPACE       = SDL_SCANCODE_KP_BACKSPACE,
    FE_KEY_KP_A               = SDL_SCANCODE_KP_A,
    FE_KEY_KP_B               = SDL_SCANCODE_KP_B,
    FE_KEY_KP_C               = SDL_SCANCODE_KP_C,
    FE_KEY_KP_D               = SDL_SCANCODE_KP_D,
    FE_KEY_KP_E               = SDL_SCANCODE_KP_E,
    FE_KEY_KP_F               = SDL_SCANCODE_KP_F,
    FE_KEY_KP_XOR             = SDL_SCANCODE_KP_XOR,
    FE_KEY_KP_POWER           = SDL_SCANCODE_KP_POWER,
    FE_KEY_KP_PERCENT         = SDL_SCANCODE_KP_PERCENT,
    FE_KEY_KP_LESS            = SDL_SCANCODE_KP_LESS,
    FE_KEY_KP_GREATER         = SDL_SCANCODE_KP_GREATER,
    FE_KEY_KP_AMPERSAND       = SDL_SCANCODE_KP_AMPERSAND,
    FE_KEY_KP_DBLAMPERSAND    = SDL_SCANCODE_KP_DBLAMPERSAND,
    FE_KEY_KP_VERTICALBAR     = SDL_SCANCODE_KP_VERTICALBAR,
    FE_KEY_KP_DBLVERTICALBAR  = SDL_SCANCODE_KP_DBLVERTICALBAR,
    FE_KEY_KP_COLON           = SDL_SCANCODE_KP_COLON,
    FE_KEY_KP_HASH            = SDL_SCANCODE_KP_HASH,
    FE_KEY_KP_SPACE           = SDL_SCANCODE_KP_SPACE,
    FE_KEY_KP_AT              = SDL_SCANCODE_KP_AT,
    FE_KEY_KP_EXCLAM          = SDL_SCANCODE_KP_EXCLAM,
    FE_KEY_KP_MEMSTORE        = SDL_SCANCODE_KP_MEMSTORE,
    FE_KEY_KP_MEMRECALL       = SDL_SCANCODE_KP_MEMRECALL,
    FE_KEY_KP_MEMCLEAR        = SDL_SCANCODE_KP_MEMCLEAR,
    FE_KEY_KP_MEMADD          = SDL_SCANCODE_KP_MEMADD,
    FE_KEY_KP_MEMSUBTRACT     = SDL_SCANCODE_KP_MEMSUBTRACT,
    FE_KEY_KP_MEMMULTIPLY     = SDL_SCANCODE_KP_MEMMULTIPLY,
    FE_KEY_KP_MEMDIVIDE       = SDL_SCANCODE_KP_MEMDIVIDE,
    FE_KEY_KP_PLUSMINUS       = SDL_SCANCODE_KP_PLUSMINUS,
    FE_KEY_KP_CLEAR           = SDL_SCANCODE_KP_CLEAR,
    FE_KEY_KP_CLEARENTRY      = SDL_SCANCODE_KP_CLEARENTRY,
    FE_KEY_KP_BINARY          = SDL_SCANCODE_KP_BINARY,
    FE_KEY_KP_OCTAL           = SDL_SCANCODE_KP_OCTAL,
    FE_KEY_KP_DECIMAL         = SDL_SCANCODE_KP_DECIMAL,
    FE_KEY_KP_HEXADECIMAL     = SDL_SCANCODE_KP_HEXADECIMAL,

    FE_KEY_LCTRL              = SDL_SCANCODE_LCTRL,
    FE_KEY_LSHIFT             = SDL_SCANCODE_LSHIFT,
    FE_KEY_LALT               = SDL_SCANCODE_LALT,
    FE_KEY_LGUI               = SDL_SCANCODE_LGUI,
    FE_KEY_RCTRL              = SDL_SCANCODE_RCTRL,
    FE_KEY_RSHIFT             = SDL_SCANCODE_RSHIFT,
    FE_KEY_RALT               = SDL_SCANCODE_RALT,
    FE_KEY_RGUI               = SDL_SCANCODE_RGUI,

    FE_KEY_MODE               = SDL_SCANCODE_MODE,

    FE_KEY_AUDIONEXT          = SDL_SCANCODE_AUDIONEXT,
    FE_KEY_AUDIOPREV          = SDL_SCANCODE_AUDIOPREV,
    FE_KEY_AUDIOSTOP          = SDL_SCANCODE_AUDIOSTOP,
    FE_KEY_AUDIOPLAY          = SDL_SCANCODE_AUDIOPLAY,
    FE_KEY_AUDIOMUTE          = SDL_SCANCODE_AUDIOMUTE,
    FE_KEY_MEDIASELECT        = SDL_SCANCODE_MEDIASELECT,
    FE_KEY_WWW                = SDL_SCANCODE_WWW,
    FE_KEY_MAIL               = SDL_SCANCODE_MAIL,
    FE_KEY_CALCULATOR         = SDL_SCANCODE_CALCULATOR,
    FE_KEY_COMPUTER           = SDL_SCANCODE_COMPUTER,
    FE_KEY_AC_SEARCH          = SDL_SCANCODE_AC_SEARCH,
    FE_KEY_AC_HOME            = SDL_SCANCODE_AC_HOME,
    FE_KEY_AC_BACK            = SDL_SCANCODE_AC_BACK,
    FE_KEY_AC_FORWARD         = SDL_SCANCODE_AC_FORWARD,
    FE_KEY_AC_STOP            = SDL_SCANCODE_AC_STOP,
    FE_KEY_AC_REFRESH         = SDL_SCANCODE_AC_REFRESH,
    FE_KEY_AC_BOOKMARKS       = SDL_SCANCODE_AC_BOOKMARKS,

    FE_KEY_BRIGHTNESSDOWN     = SDL_SCANCODE_BRIGHTNESSDOWN,
    FE_KEY_BRIGHTNESSUP       = SDL_SCANCODE_BRIGHTNESSUP,
    FE_KEY_DISPLAYSWITCH      = SDL_SCANCODE_DISPLAYSWITCH,
    FE_KEY_KBDILLUMTOGGLE     = SDL_SCANCODE_KBDILLUMTOGGLE,
    FE_KEY_KBDILLUMDOWN       = SDL_SCANCODE_KBDILLUMDOWN,
    FE_KEY_KBDILLUMUP         = SDL_SCANCODE_KBDILLUMUP,
    FE_KEY_EJECT              = SDL_SCANCODE_EJECT,
    FE_KEY_SLEEP              = SDL_SCANCODE_SLEEP,

    FE_KEY_APP1               = SDL_SCANCODE_APP1,
    FE_KEY_APP2               = SDL_SCANCODE_APP2,

    FE_KEY_AUDIOREWIND        = SDL_SCANCODE_AUDIOREWIND,
    FE_KEY_AUDIOFASTFORWARD   = SDL_SCANCODE_AUDIOFASTFORWARD,

    FE_NUM_SCANCODES          = SDL_NUM_SCANCODES
} FE_KEY;

#define FE_BUTTON(X)       (1 << ((X)-1))
#define FE_BUTTON_LEFT     1
#define FE_BUTTON_MIDDLE   2
#define FE_BUTTON_RIGHT    3
#define FE_BUTTON_X1       4
#define FE_BUTTON_X2       5
#define FE_BUTTON_LMASK    FE_BUTTON(SDL_BUTTON_LEFT)
#define FE_BUTTON_MMASK    FE_BUTTON(SDL_BUTTON_MIDDLE)
#define FE_BUTTON_RMASK    FE_BUTTON(SDL_BUTTON_RIGHT)
#define FE_BUTTON_X1MASK   FE_BUTTON(SDL_BUTTON_X1)
#define FE_BUTTON_X2MASK   FE_BUTTON(SDL_BUTTON_X2)

typedef void (*fe_input_key_down_fn)(FE_KEY key);
typedef void (*fe_input_key_up_fn)(FE_KEY key);
typedef void (*fe_input_mouse_button_down_fn)(u8 button);
typedef void (*fe_input_mouse_button_up_fn)(u8 button);
typedef void (*fe_input_mouse_move_fn)(const fe_vec2_t *motion);
typedef void (*fe_input_mouse_scroll_fn)(const fe_vec2_t *wheel);

void      fe_input_setup(void);
void      fe_input_cleanup(void);
void      fe_input_reset(void);
void      fe_input_process_sdl_event(SDL_Event *e);
void      fe_input_lock_mouse(bool lock);
void      fe_input_lock_mouse_to_window(bool lock);
fe_vec2_t fe_input_get_mouse_position(void);
fe_vec2_t fe_input_get_mouse_motion(void);
fe_vec3_t fe_input_get_mouse_motion_rotation(void);
fe_vec2_t fe_input_get_mouse_scroll(void);
bool      fe_input_did_mouse_scroll(void);
bool      fe_input_did_mouse_move(void);
bool      fe_input_is_key_pressed(FE_KEY key);
bool      fe_input_is_key_down(FE_KEY key);
bool      fe_input_is_mouse_button_down(u8 button);

void fe_input_add_key_down_callback(fe_input_key_down_fn cb);
void fe_input_add_key_up_callback(fe_input_key_up_fn cb);
void fe_input_add_mouse_button_down_callback(fe_input_mouse_button_down_fn cb);
void fe_input_add_mouse_move_callback(fe_input_mouse_move_fn cb);
void fe_input_add_mouse_scroll_callback(fe_input_mouse_scroll_fn cb);

void fe_input_del_key_down_callback(fe_input_key_down_fn cb);
void fe_input_del_key_up_callback(fe_input_key_up_fn cb);
void fe_input_del_mouse_button_down_callback(fe_input_mouse_button_down_fn cb);
void fe_input_del_mouse_button_up_callback(fe_input_mouse_button_up_fn cb);
void fe_input_del_mouse_move_callback(fe_input_mouse_move_fn cb);
void fe_input_del_mouse_scroll_callback(fe_input_mouse_scroll_fn cb);


fe_ray_t  fe_ray_from_mouse(const fe_vec3_t *orig, const fe_mat4x4_t *view, const fe_mat4x4_t *proj);



