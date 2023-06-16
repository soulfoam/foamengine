#pragma once

#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/return_code.h>

typedef enum
{
	ED_CAMERA_INPUT_FORWARD  = 1 << 0,
	ED_CAMERA_INPUT_BACKWARD = 1 << 1,
	ED_CAMERA_INPUT_LEFT     = 1 << 2,
	ED_CAMERA_INPUT_RIGHT    = 1 << 3,
	ED_CAMERA_INPUT_UP       = 1 << 4,
	ED_CAMERA_INPUT_DOWN     = 1 << 5,
	ED_CAMERA_INPUT_SPEED_UP  = 1 << 6,
	ED_CAMERA_INPUT_SPEED_DOWN = 1 << 7,

} ED_CAMERA_INPUT_FLAGS;

typedef struct 
{
	f32 fov;
	f32 near_clip;
	f32 far_clip;

    f32 speed;

    u16 flags;
} ed_camera_t;

void ed_camera_init(ed_camera_t *camera);
void ed_camera_get_projection_matrix(ed_camera_t *camera, fe_mat4x4_t *dst);

