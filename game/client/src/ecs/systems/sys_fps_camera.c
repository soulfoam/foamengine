#include <client/client.h>
#include <foam/foam_client.h>

static void
push_view_and_projection(void)
{
    cl_game_manager_t *gm = cl_game_manager_get_ptr();

    fe_transform_t *xform           = fe_entity_get_component(gm->cam, FE_COMPONENT_TRANSFORM);
    fe_transform_view_t *xform_view = fe_entity_get_component(gm->cam, FE_COMPONENT_TRANSFORM_VIEW);
    cl_fps_camera_t *cam  = fe_entity_get_component(gm->cam, CL_COMPONENT_FPS_CAMERA);

    fe_mat4x4_t proj;
    cl_fps_camera_get_projection_matrix(cam, &proj);

    //fe_vec3_t front, up;
    //fe_transform_get_forward(xform, &front);
    //fe_transform_get_up(xform, &up);

    //const fe_vec3_t *world_trl = fe_transform_get_world_translation(xform);

    //fe_vec3_t center;
    //fe_vec3_add_to(&center, world_trl, &front);

    fe_mat4x4_t view;
    //view = fe_mat4x4_look_at(world_trl, &center, &up);

    fe_transform_generate_world_view_matrix(fe_transform_view_get_transform(xform_view), &view);

    fe_renderer_push_view_matrix(&view);
    fe_renderer_push_projection_matrix(&proj);

}

static void
pop_view_and_projection(void)
{
    fe_renderer_pop_view_matrix();
    fe_renderer_pop_projection_matrix();
}

void
cl_system_fps_camera_tick_pre(void)
{
    push_view_and_projection();
}

void
cl_system_fps_camera_tick_post(void)
{
    pop_view_and_projection();
} 

void
cl_system_fps_camera_fixed_tick(void)
{
    cl_input_manager_t *im = cl_input_manager_get_ptr();
    cl_game_manager_t *gm  = cl_game_manager_get_ptr();

    cl_fps_camera_t *cam  = fe_entity_get_component(gm->cam, CL_COMPONENT_FPS_CAMERA);
    fe_transform_t *xform = fe_entity_get_component(gm->cam, FE_COMPONENT_TRANSFORM);

    const f32 tilt_smooth = 0.2f;

    f32 targ = cam->tilt_target - cam->tilt;
    cam->tilt += targ * tilt_smooth;
    cam->tilt = fe_math_clamp(cam->tilt, -cam->tilt_amt, cam->tilt_amt);

    fe_vec3_t rot = im->cam_rot;
    rot.y = cam->tilt;
    fe_transform_rotate_euler(xform, &rot);


}

void
cl_system_fps_camera_weapon_fixed_tick(void)
{ 
    cl_game_manager_t *gm = cl_game_manager_get_ptr();
    cl_input_manager_t *im = cl_input_manager_get_ptr();

    cl_fps_camera_t *cam  = fe_entity_get_component(gm->cam, CL_COMPONENT_FPS_CAMERA);
    fe_transform_t *wep_xform = fe_entity_get_component(gm->weapon, FE_COMPONENT_TRANSFORM);

    fe_vec3_t rot = im->cam_rot;
    rot.y = cam->tilt;

   // if (rot.x <= -20) rot.x = -20;
   // if (rot.x >= 20) rot.x  = 20;

    fe_transform_rotate_euler(wep_xform, &rot);
}
