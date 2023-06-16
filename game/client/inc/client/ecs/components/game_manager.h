#pragma once

#include <foam/foam_client.h>
#include <client/client.h>

typedef struct
{
    fe_entity_id cam;
    fe_entity_id crosshair;

    fe_entity_id weapon; //just for visual for now

    fe_entity_id map;

    bool debug_mode;

    fe_entity_id local_player;

} cl_game_manager_t;

void               cl_game_manager_setup(void);
void               cl_game_manager_cleanup(void);
cl_game_manager_t* cl_game_manager_get_ptr(void);

