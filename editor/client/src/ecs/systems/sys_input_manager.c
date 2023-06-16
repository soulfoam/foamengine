#include <client/editor.h>

void
ed_input_manager_on_mouse_move(const fe_vec2_t *motion)
{
    ed_input_manager_t *im = ed_input_manager_get_ptr();
    if (!im->enabled) return;

    im->cam_rot.x -= motion->y * im->mouse_sens;
    im->cam_rot.z -= motion->x * im->mouse_sens; 

    if      (im->cam_rot.x > 89.0f)  im->cam_rot.x = 89.0f;
    else if (im->cam_rot.x < -89.0f) im->cam_rot.x = -89.0f;
}

void
ed_input_manager_on_mouse_button_down(u8 button)
{
    ed_input_manager_t *im = ed_input_manager_get_ptr();
    //if (!im->enabled) return;

    ed_manager_t *em = ed_manager_get_ptr();

    if (button == FE_BUTTON_LEFT)
    {
        fe_transform_t *cam_xform = fe_entity_get_component(em->cam, FE_COMPONENT_TRANSFORM);

        fe_ray_t ray;
        fe_transform_get_world_ray(cam_xform, &ray);

        fe_transform_t *xform = fe_entity_get_component(ed_manager_get_ptr()->cam, FE_COMPONENT_TRANSFORM);
    
        //fe_ray_t ray = fe_ray_from_mouse(&xform->world_trl, fe_renderer_get_view_matrix(), fe_renderer_get_projection_matrix());

        //fe_log("mouse");
        //fe_vec3_print(&ray.orig);
        //fe_vec3_print(&ray.dir);
        //fe_log("xform");
        //fe_vec3_print(&rayx.orig);
        //fe_vec3_print(&rayx.dir);

        f32 dist;
        f32 lowest_dist = FLT_MAX;
        fe_entity_id closest_ent = (fe_entity_id){FE_INVALID_ID};
        fe_ecs_query(EDITOR_QUERY_EDITABLES, e)
        {
            ed_editable_t *editable = fe_entity_get_component(*e, EDITOR_COMPONENT_EDITABLE);

            if (fe_ray_vs_aabb(&ray, &editable->aabb, &dist))
            {
                fe_log("hitting %s | dist: %f | lowest: %f", fe_entity_get_name(*e), dist, lowest_dist);
                if (fabs(dist) <= fabs(lowest_dist))
                {
                    fe_log("did %s | fabs_dist: %f | fabs_lowest: %f", fe_entity_get_name(*e), fabs(dist), fabs(lowest_dist));
                    lowest_dist = dist; 

                    closest_ent = *e;
                }
            }
        }    

        if (FE_VALID_HANDLE_ID(closest_ent))   
        {
            ed_editable_t *editable   = fe_entity_get_component(closest_ent, EDITOR_COMPONENT_EDITABLE);
            fe_transform_t *xform     = fe_entity_get_component(closest_ent, FE_COMPONENT_TRANSFORM);
            editable->selected        = !editable->selected;
            editable->dist_from_ray   = fabs(lowest_dist);
            editable->orig_pickup_trl = xform->trl;

        }
    }


    //clean all this up, we basically want the manager to have different modes too
    //so the for each type of keypress we can do the right thing
    if (button == FE_BUTTON_LEFT)
    {
        
    }
}

void
ed_input_manager_on_key_up(FE_KEY key)
{
    FE_SCREEN_MODE mode = fe_window_get_screen_mode();

    ed_input_manager_t *im = ed_input_manager_get_ptr();

	switch ((i32)key)
	{
		case FE_KEY_V:
            fe_window_set_vsync(!fe_window_get_vsync());
        break;

		case FE_KEY_ESCAPE:
            fe_input_lock_mouse(false);
            im->enabled = false;
		break;

		case FE_KEY_L:
            fe_input_lock_mouse(true);
            im->enabled = true;
		break;

		case FE_KEY_F:
            if (mode == FE_SCREEN_MODE_WINDOWED) fe_window_set_screen_mode(FE_SCREEN_MODE_FULLSCREEN);
            else                                 fe_window_set_screen_mode(FE_SCREEN_MODE_WINDOWED);
		break;

    }
}

static void
camera_input(void)
{
    ed_input_manager_t *im = ed_input_manager_get_ptr();

    if (fe_input_did_mouse_scroll())
    {
        fe_vec2_t mouse_scroll = fe_input_get_mouse_scroll();
        im->cam_zoom += mouse_scroll.y;
    }

    ed_camera_t *cam = fe_entity_get_component(ed_manager_get_ptr()->cam, EDITOR_COMPONENT_CAMERA);

    cam->flags = 0;
    cam->fov = im->cam_fov + im->cam_zoom;

    const uint8_t *k = SDL_GetKeyboardState(NULL);
    if (k[FE_KEY_W])      cam->flags |=  ED_CAMERA_INPUT_FORWARD;  
    if (k[FE_KEY_S])      cam->flags |=  ED_CAMERA_INPUT_BACKWARD;
    if (k[FE_KEY_A])      cam->flags |=  ED_CAMERA_INPUT_LEFT;    
    if (k[FE_KEY_D])      cam->flags |=  ED_CAMERA_INPUT_RIGHT; 
    if (k[FE_KEY_SPACE])  cam->flags |=  ED_CAMERA_INPUT_UP;     
    if (k[FE_KEY_LSHIFT]) cam->flags |=  ED_CAMERA_INPUT_DOWN;     
    if (k[FE_KEY_Q])      cam->flags |=  ED_CAMERA_INPUT_SPEED_DOWN;     
    if (k[FE_KEY_E])      cam->flags |=  ED_CAMERA_INPUT_SPEED_UP;     

}


void 
ed_system_input_manager_tick(void)
{
    ed_manager_t *em = ed_manager_get_ptr();
        
    camera_input();

}

void 
ed_system_input_manager_fixed_tick(void)
{
    ed_manager_t *em = ed_manager_get_ptr();

}

