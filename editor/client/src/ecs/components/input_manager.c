#include <client/ecs/ecs_info.h>

static ed_input_manager_t mngr;

void 
ed_input_manager_setup(void)
{
    mngr.player_input = (fe_entity_id){FE_INVALID_ID};
    mngr.cam_rot      = FE_VEC3_ZERO;
    mngr.mouse_sens   = 0.5f;
    mngr.cam_fov      = 60;
    mngr.cam_zoom     = 0;
    mngr.enabled      = true;

    fe_input_lock_mouse(true);

    fe_input_add_mouse_move_callback(ed_input_manager_on_mouse_move);
    fe_input_add_mouse_button_down_callback(ed_input_manager_on_mouse_button_down);
    fe_input_add_key_up_callback(ed_input_manager_on_key_up);
}

void 
ed_input_manager_cleanup(void)
{

}

ed_input_manager_t*
ed_input_manager_get_ptr(void)
{
    return &mngr;
}

