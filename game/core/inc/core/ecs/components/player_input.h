#pragma once

#include <foam/foam_core.h>

typedef enum
{
    CO_PLAYER_INPUT_FORWARD  = 1 << 0,
    CO_PLAYER_INPUT_BACKWARD = 1 << 1,
    CO_PLAYER_INPUT_LEFT     = 1 << 2,
    CO_PLAYER_INPUT_RIGHT    = 1 << 3,
    CO_PLAYER_INPUT_JUMP     = 1 << 4,
    CO_PLAYER_INPUT_ATTACK_L = 1 << 5,
    CO_PLAYER_INPUT_ATTACK_R = 1 << 6,
    CO_PLAYER_INPUT_ITEM_0   = 1 << 7,
    CO_PLAYER_INPUT_ITEM_1   = 1 << 8,
    CO_PLAYER_INPUT_ITEM_2   = 1 << 9,
    CO_PLAYER_INPUT_ITEM_3   = 1 << 10,
    CO_PLAYER_INPUT_DASH     = 1 << 11,
    CO_PLAYER_INPUT_CROUCH   = 1 << 12,
} CO_PLAYER_INPUT_FLAG;

typedef struct
{
    u16       flags;
    u16       prev_flags;
    fe_vec3_t rotation;
    f32       jump_timer;
    bool      auto_bunny_hop;
} co_player_input_t;

void co_player_input_init(co_player_input_t *pi);

