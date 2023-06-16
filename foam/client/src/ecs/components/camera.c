#include <foam/client/gfx/window.h>
#include <foam/client/ecs/components/camera.h>
#include <foam/core/ext/logger.h>

static const f32 FOV                = 60.0f;
static const f32 NEAR_CLIP          = 0.1f;
static const f32 FAR_CLIP           = 2000.0f;
static const f32 DISTANCE_FROM_TARG = 15.0f;
static const f32 YAW                = 90.0;
static const f32 PITCH              = 0.0f;
static const f32 SPEED              = 0.012f;
static const f32 SENSITIVITY        = 4.0f;
static const f32 ZOOM               = 0.0f;

void
fe_camera_init(fe_camera_t *cam)
{
	cam->fov         = FOV;
	cam->near_clip   = NEAR_CLIP;
	cam->far_clip    = FAR_CLIP;
}

void
fe_camera_get_projection_matrix(fe_camera_t *cam, fe_mat4x4_t *dst)
{
    const fe_vec2_t *win_size = fe_window_get_size();

    *dst = fe_mat4x4_perspective
    (
        fe_math_rad(cam->fov),
        (win_size->x / win_size->y),
        cam->near_clip,
        cam->far_clip
    );
}

