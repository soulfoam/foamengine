#include <client/editor.h>
#include <foam/foam_client.h>

static void
push_view_and_projection(void)
{
    ed_manager_t *em = ed_manager_get_ptr();

    fe_transform_view_t *xform_view = fe_entity_get_component(em->cam, FE_COMPONENT_TRANSFORM_VIEW);
    ed_camera_t *cam  = fe_entity_get_component(em->cam, EDITOR_COMPONENT_CAMERA);

    fe_mat4x4_t proj;
    ed_camera_get_projection_matrix(cam, &proj);

    fe_mat4x4_t view;
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
ed_system_camera_tick_pre(void)
{
    push_view_and_projection();
}

void
ed_system_camera_tick_post(void)
{
    pop_view_and_projection();
} 


void
ed_system_camera_fixed_tick(void)
{
    ed_input_manager_t *im = ed_input_manager_get_ptr();
    ed_manager_t *em       = ed_manager_get_ptr();
    fe_transform_t *xform = fe_entity_get_component(em->cam, FE_COMPONENT_TRANSFORM);
    ed_camera_t *cam = fe_entity_get_component(em->cam, EDITOR_COMPONENT_CAMERA);

    if (cam->flags & ED_CAMERA_INPUT_SPEED_DOWN) cam->speed -= 0.5f; 
    if (cam->flags & ED_CAMERA_INPUT_SPEED_UP)   cam->speed += 0.5f;  
    if (cam->speed <= 1.0f) cam->speed = 1.0f;

    fe_vec3_t front, right, up;
    fe_transform_get_forward(xform, &front);
    fe_transform_get_right(xform, &right);
    fe_transform_get_up(xform, &up);

    fe_vec3_scale(&front, cam->speed);
    fe_vec3_scale(&right, cam->speed);
    fe_vec3_scale(&up,    cam->speed);

    fe_vec3_t trl = FE_VEC3_ZERO;
    if (cam->flags & ED_CAMERA_INPUT_FORWARD)  fe_vec3_add(&trl, &front);
    if (cam->flags & ED_CAMERA_INPUT_BACKWARD) fe_vec3_sub(&trl, &front);
    if (cam->flags & ED_CAMERA_INPUT_RIGHT)    fe_vec3_add(&trl, &right);
    if (cam->flags & ED_CAMERA_INPUT_LEFT)     fe_vec3_sub(&trl, &right);
    if (cam->flags & ED_CAMERA_INPUT_UP)       fe_vec3_add(&trl, &up);
    if (cam->flags & ED_CAMERA_INPUT_DOWN)     fe_vec3_sub(&trl, &up);


    fe_transform_translate(xform, &trl);

    fe_transform_rotate_euler(xform, &im->cam_rot);
}
