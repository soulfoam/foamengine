#include <client/ecs/components/input_manager.h>

static cl_input_manager_t mngr;

static void
input_manager_on_mouse_move(const fe_vec2_t *motion)
{
    mngr.cam_rot.x -= motion->y * mngr.mouse_sens;
    mngr.cam_rot.z -= motion->x * mngr.mouse_sens; 

    if      (mngr.cam_rot.x > 89.0f)  mngr.cam_rot.x = 89.0f;
    else if (mngr.cam_rot.x < -89.0f) mngr.cam_rot.x = -89.0f;
}

void 
cl_input_manager_setup(void)
{
    mngr.player_input = (fe_entity_id){FE_INVALID_ID};
    mngr.cam_rot      = FE_VEC3_ZERO;
    mngr.mouse_sens   = 0.5f;
    mngr.cam_fov      = 60;
    mngr.cam_zoom     = 0;
    mngr.flags = 0;

    fe_input_add_mouse_move_callback(input_manager_on_mouse_move);
}

void 
cl_input_manager_cleanup(void)
{

}

cl_input_manager_t*
cl_input_manager_get_ptr(void)
{
    return &mngr;
}

