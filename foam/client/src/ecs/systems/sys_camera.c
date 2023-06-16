#include <foam/client/engine.h>
#include <foam/client/input.h>

void 
fe_client_system_camera_pre_draw(void)
{
    fe_ecs_query(FE_QUERY_CAMERA, e)
    {    
        fe_transform_t *xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);
        fe_camera_t *cam      = fe_entity_get_component(*e, FE_COMPONENT_CAMERA);

        fe_mat4x4_t view, proj;
        fe_camera_get_projection_matrix(cam, &proj);

        //fe_transform_generate_world_view_matrix(xform, &view);

        fe_vec3_t front, up;
        fe_transform_get_forward(xform, &front);
        fe_transform_get_up(xform, &up);

        fe_vec3_t center; 
        fe_vec3_add_to(&center, fe_transform_get_world_translation(xform), &front);

        view = fe_mat4x4_look_at(fe_transform_get_world_translation(xform), &center, &up);


        //BIG BUG HERE, IT APPLIES VP MATRIX FOR EVERY CAMERA (so the last camera wins >:] )
        //WE WANT ONLY FOR OUR ACTIVE CAMERA!
        
        fe_renderer_push_view_matrix(&view);
        fe_renderer_push_projection_matrix(&proj);
    }
}

void 
fe_client_system_camera_post_draw(void)
{
    fe_ecs_query(FE_QUERY_CAMERA, e)
    {    
        fe_renderer_pop_view_matrix();
        fe_renderer_pop_projection_matrix();
    }
}


