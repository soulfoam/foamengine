#include <client/client.h>

void
cl_system_(void)
{

}

void 
cl_system_game_manager_tick(void)
{
    cl_game_manager_t *gm = cl_game_manager_get_ptr();

    if (fe_input_is_key_pressed(FE_KEY_P))
    {
        gm->debug_mode = !gm->debug_mode;
    }

    if (fe_input_is_key_pressed(FE_KEY_H))
    {
        co_player_input_t *pi = fe_entity_get_component(gm->local_player, CO_COMPONENT_PLAYER_INPUT);
        pi->auto_bunny_hop= !pi->auto_bunny_hop;
    }

    if (fe_input_is_key_pressed(FE_KEY_M))
    {
        if (!cl_debug_manager_get_ptr()->show_mesh_colliders)
        {
            cl_debug_manager_enable_mesh_colliders();
            cl_debug_manager_enable_simple_colliders();
            cl_debug_manager_enable_rigidbodies();
        }
        else
        {
            cl_debug_manager_disable_mesh_colliders();
            cl_debug_manager_disable_simple_colliders();
            cl_debug_manager_disable_rigidbodies();
        }
    }

    fe_transform_t *xform = fe_entity_get_component(gm->weapon, FE_COMPONENT_TRANSFORM);
    fe_transform_view_t *view_xform = fe_entity_get_component(gm->weapon, FE_COMPONENT_TRANSFORM_VIEW);


    //fe_mat4x4_print(&((fe_transform_t*)fe_entity_get_component(gm->weapon, FE_COMPONENT_TRANSFORM))->world_matrix);
    //fe_vec3_t trl = *fe_transform_get_world_translation(cam_xform);
    //trl.y += 4;
    //trl.z -= 1;
    //fe_vec3_t forward;
    //fe_transform_get_forward(cam_xform, &forward);
    //forward.x = 0;
    //fe_transform_set_translation(wep_xform, &trl);
    //fe_transform_rotate_euler(wep_xform, &forward);
}

