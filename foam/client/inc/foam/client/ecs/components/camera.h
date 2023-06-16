#pragma once

#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/return_code.h>

typedef struct 
{
	f32 fov;
	f32 near_clip;
	f32 far_clip;
} fe_camera_t;

void fe_camera_init(fe_camera_t *cam);
void fe_camera_get_projection_matrix(fe_camera_t *cam, fe_mat4x4_t *dst);
