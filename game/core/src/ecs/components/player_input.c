#include <core/ecs/components/player_input.h>

void
co_player_input_init(co_player_input_t *pi)
{
    pi->flags        = 0;
    pi->prev_flags   = 0;
    pi->rotation     = FE_VEC3_ZERO;
    pi->jump_timer   = 0;
    pi->auto_bunny_hop = false;
}

