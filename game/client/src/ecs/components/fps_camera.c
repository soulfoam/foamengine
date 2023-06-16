#include <foam/foam_client.h>
#include <client/ecs/components/fps_camera.h>

void
cl_fps_camera_init(cl_fps_camera_t *cam)
{
	cam->fov         = 60.0f;
	cam->near_clip   = 0.1f;
	cam->far_clip    = 10000.0f;
    cam->tilt_amt    = 2;
    cam->tilt        = 0;
    cam->tilt_target = 0;
}

void
cl_fps_camera_get_projection_matrix(cl_fps_camera_t *cam, fe_mat4x4_t *dst)
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



