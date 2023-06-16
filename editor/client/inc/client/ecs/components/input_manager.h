#pragma once

#include <foam/foam_client.h>


typedef struct
{
    //we can keep our input buffer history here with each tick and what our transform was
    fe_entity_id player_input; //local input

    fe_vec3_t   cam_rot;

    f32         cam_fov; //get this out of here once we have an actual gamesettings component

    f32         mouse_sens;
    f32         cam_zoom;

    bool        enabled;
} ed_input_manager_t;

void                   ed_input_manager_setup(void);
void                   ed_input_manager_cleanup(void);
ed_input_manager_t*    ed_input_manager_get_ptr(void);

