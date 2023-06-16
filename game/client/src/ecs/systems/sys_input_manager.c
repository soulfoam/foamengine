#include <client/client.h>

static f32 rot = 0.0f;

static void
handle_camera(void)
{
    cl_input_manager_t *im = cl_input_manager_get_ptr();

    fe_transform_t *xform = fe_entity_get_component(cl_game_manager_get_ptr()->cam, FE_COMPONENT_TRANSFORM);

    f32 velocity = 1;

    fe_vec3_t front, right, up;
    fe_transform_get_forward(xform, &front);
    fe_transform_get_right(xform, &right);
    fe_transform_get_up(xform, &up);

    fe_vec3_scale(&front, velocity);
    fe_vec3_scale(&right, velocity);
    fe_vec3_scale(&up,    velocity);

    fe_vec3_t trl = FE_VEC3_ZERO;
    if (im->flags & CAMERA_INPUT_FORWARD)  fe_vec3_add(&trl, &front);
    if (im->flags & CAMERA_INPUT_BACKWARD) fe_vec3_sub(&trl, &front);
    if (im->flags & CAMERA_INPUT_RIGHT)    fe_vec3_add(&trl, &right);
    if (im->flags & CAMERA_INPUT_LEFT)     fe_vec3_sub(&trl, &right);
    if (im->flags & CAMERA_INPUT_UP)       fe_vec3_add(&trl, &up);
    if (im->flags & CAMERA_INPUT_DOWN)     fe_vec3_sub(&trl, &up);
    fe_transform_translate(xform, &trl);

    //fe_transform_rotate_euler(xform, &im->cam_rot);
}

static void
handle_camera_input(void)
{
    cl_input_manager_t *im = cl_input_manager_get_ptr();
    im->flags = 0;

    if (fe_input_did_mouse_scroll())
    {
        fe_vec2_t mouse_scroll = fe_input_get_mouse_scroll();
        im->cam_zoom += mouse_scroll.y;
    }

    fe_camera_t *cam = fe_entity_get_component(cl_game_manager_get_ptr()->cam, FE_COMPONENT_CAMERA);
    cam->fov = im->cam_fov + im->cam_zoom;

    const uint8_t *k = SDL_GetKeyboardState(NULL);
    if (k[FE_KEY_W])      im->flags |=  CAMERA_INPUT_FORWARD;  
    if (k[FE_KEY_S])      im->flags |=  CAMERA_INPUT_BACKWARD;
    if (k[FE_KEY_A])      im->flags |=  CAMERA_INPUT_LEFT;    
    if (k[FE_KEY_D])      im->flags |=  CAMERA_INPUT_RIGHT; 
    if (k[FE_KEY_SPACE])  im->flags |=  CAMERA_INPUT_UP;     
    if (k[FE_KEY_LSHIFT]) im->flags |=  CAMERA_INPUT_DOWN;     
}

static void
player_input_fixed_tick(void)
{
    cl_game_manager_t *gm = cl_game_manager_get_ptr();
    co_player_input_t *mv  = fe_entity_get_component(gm->local_player, CO_COMPONENT_PLAYER_INPUT);
    if (!mv) return;

    cl_input_manager_t *im = cl_input_manager_get_ptr();
    mv->rotation           = im->cam_rot;
}

static void
player_input_tick(void)
{
    cl_game_manager_t *gm = cl_game_manager_get_ptr();
    co_player_input_t *mv = fe_entity_get_component(gm->local_player, CO_COMPONENT_PLAYER_INPUT);
    cl_fps_camera_t *cam = fe_entity_get_component(gm->cam, CL_COMPONENT_FPS_CAMERA);

    const uint8_t *k = SDL_GetKeyboardState(NULL);

    //input manager should obviously do keybinds and shit eventually
    if (mv)
    {
        if (k[FE_KEY_W])     mv->flags |= CO_PLAYER_INPUT_FORWARD;
        if (k[FE_KEY_S])     mv->flags |= CO_PLAYER_INPUT_BACKWARD;
        if (k[FE_KEY_A])     mv->flags |= CO_PLAYER_INPUT_LEFT;
        if (k[FE_KEY_D])     mv->flags |= CO_PLAYER_INPUT_RIGHT;
        if (k[FE_KEY_SPACE]) mv->flags |= CO_PLAYER_INPUT_JUMP;
    }

    if (cam)
    {        
        if      (k[FE_KEY_A]) cam->tilt_target = -cam->tilt_amt;
        else if (k[FE_KEY_D]) cam->tilt_target = cam->tilt_amt;
        else                  cam->tilt_target = 0;
    }


    if (fe_input_is_key_down(FE_KEY_ESCAPE))
    {
        fe_input_lock_mouse(false);
    }

    if (fe_input_is_key_down(FE_KEY_1))
    {
        fe_input_lock_mouse(true);
    }
}

void 
cl_system_input_manager_tick(void)
{
    cl_game_manager_t *gm = cl_game_manager_get_ptr();
        
    player_input_tick();

}

void 
cl_system_input_manager_fixed_tick(void)
{
    //handle_camera();
    player_input_fixed_tick();

    cl_game_manager_t *gm = cl_game_manager_get_ptr();

    fe_transform_t *xform = fe_entity_get_component(gm->cam, FE_COMPONENT_TRANSFORM);

    fe_ray_t ray;
    fe_transform_get_world_ray(xform, &ray);
    fe_entity_id hit = fe_ray_vs_simple_colliders(&ray, NULL);

    if (FE_VALID_HANDLE_ID(hit))
    {
        fe_log("Hit %s", fe_entity_get_name(hit)); 
    }
}

