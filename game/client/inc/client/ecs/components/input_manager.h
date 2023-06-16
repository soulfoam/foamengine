#pragma once

#include <foam/foam_client.h>
#include <core/core.h>

typedef enum
{
	CAMERA_INPUT_FORWARD  = 1 << 0,
	CAMERA_INPUT_BACKWARD = 1 << 1,
	CAMERA_INPUT_LEFT     = 1 << 2,
	CAMERA_INPUT_RIGHT    = 1 << 3,
	CAMERA_INPUT_UP       = 1 << 4,
	CAMERA_INPUT_DOWN     = 1 << 5,

} CAMERA_INPUT_FLAGS;

typedef struct
{
    //we can keep our input buffer history here with each tick and what our transform was
    fe_entity_id player_input; //local input

    u16 flags;
    fe_vec3_t   cam_rot;

    f32         cam_fov; //get this out of here once we have an actual gamesettings component

    f32         mouse_sens;
    f32         cam_zoom;
} cl_input_manager_t;

void                   cl_input_manager_setup(void);
void                   cl_input_manager_cleanup(void);
cl_input_manager_t*    cl_input_manager_get_ptr(void);

