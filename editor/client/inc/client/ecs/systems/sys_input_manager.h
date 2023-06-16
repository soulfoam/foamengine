#pragma once

#include <foam/foam_client.h>

void ed_system_input_manager_tick(void);
void ed_system_input_manager_fixed_tick(void);

//callbacks
void ed_input_manager_on_mouse_move(const fe_vec2_t *motion);
void ed_input_manager_on_mouse_button_down(u8 button);
void ed_input_manager_on_key_up(FE_KEY key);
