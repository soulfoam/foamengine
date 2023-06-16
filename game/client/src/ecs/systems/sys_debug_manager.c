#include <client/client.h>

static f32 highest_speed = 0;

void 
cl_system_debug_manager_fixed_tick(void)
{
    cl_debug_manager_t *dm = cl_debug_manager_get_ptr();
    cl_game_manager_t *gm = cl_game_manager_get_ptr();

    fe_rigidbody_t *rb     = fe_entity_get_component(gm->local_player, FE_COMPONENT_RIGIDBODY);

    fe_text2d_t *text         = fe_entity_get_component(dm->stats_text_ent, FE_COMPONENT_TEXT2D);
    fe_transform_t *cam_xform = fe_entity_get_component(cl_game_manager_get_ptr()->cam, FE_COMPONENT_TRANSFORM);
    co_player_input_t *pi = fe_entity_get_component(gm->local_player, CO_COMPONENT_PLAYER_INPUT);

    char buf[512]             = {0};

    //text changes is so fuckin slow its insane, literally lags the game 
    //dm->text_update_timer++;
    //if (dm->text_update_timer >= fe_core_get_tick_rate()/2)
    {
        dm->text_update_timer = 0;

        fe_text2d_clear(text);

        sprintf(buf, "FPS: %u | DT: %f | Entities: %u", fe_engine_get_fps(), FE_DT, fe_ecs_get_entity_count());
        fe_text2d_print(text, buf, &fe_vec2(0, 0));

        sprintf(buf, "X: %.3f | Y: %.3f | Z: %.3f", cam_xform->world_trl.x, cam_xform->world_trl.y, cam_xform->world_trl.z);
        fe_text2d_print(text, buf, &fe_vec2(0, 1));

        f32 speed = fe_vec3_len(&rb->velocity) * 100;
        if (speed >= highest_speed) highest_speed = speed;
        sprintf(buf, "Speed: %f | Highest: %f", speed, highest_speed);
        fe_text2d_print(text, buf, &fe_vec2(0, 2));

        sprintf(buf, "Auto Bunny Hop: %u", pi->auto_bunny_hop);
        fe_text2d_print(text, buf, &fe_vec2(0, 3));

    }

}

