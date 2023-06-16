#include <foam/foam_client.h>
#include <client/ecs/components/camera.h>

void
ed_camera_init(ed_camera_t *cam)
{
	cam->fov         = 60.0f;
	cam->near_clip   = 0.1f;
	cam->far_clip    = 10000.0f;
    cam->flags       = 0;
    cam->speed       = 2.0f;
}

void
ed_camera_get_projection_matrix(ed_camera_t *cam, fe_mat4x4_t *dst)
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


