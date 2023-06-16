#pragma once

#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/return_code.h>

typedef struct 
{
	f32 fov;
	f32 near_clip;
	f32 far_clip;


    f32 tilt_amt;

    f32 tilt;
    f32 tilt_target;
} cl_fps_camera_t;

void cl_fps_camera_init(cl_fps_camera_t *camera);
void cl_fps_camera_get_projection_matrix(cl_fps_camera_t *camera, fe_mat4x4_t *dst);

