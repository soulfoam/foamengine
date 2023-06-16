#include <foam/foam_core.h>
#include <core/core.h>

static void 
accelerate(fe_rigidbody_t *rb, const fe_vec3_t *dir, f32 accel, f32 max_vel) 
{
    float proj_vel = fe_vec3_dot(&rb->velocity, dir); 

    if((proj_vel + accel) > max_vel) accel = max_vel - proj_vel;

    fe_vec3_t add_vel = *dir;
    fe_vec3_scale(&add_vel, accel);
    fe_rigidbody_add_velocity(rb, &add_vel);

}

static bool
can_jump(co_player_input_t *pi)
{
    if (pi->auto_bunny_hop)
    {
        if (pi->flags & CO_PLAYER_INPUT_JUMP) 
            return true;
    }
    else
    {
        if ( ((pi->flags & CO_PLAYER_INPUT_JUMP) && !(pi->prev_flags & CO_PLAYER_INPUT_JUMP))) 
            return true;
    }

   return false;
} 

void 
co_system_player_input_handle_fixed(void)
{
    fe_ecs_query(CO_QUERY_PLAYER_INPUTS, e)
    {
        co_player_input_t *pi    = fe_entity_get_component(*e, CO_COMPONENT_PLAYER_INPUT);
        fe_rigidbody_t *rb       = fe_entity_get_component(*e, FE_COMPONENT_RIGIDBODY);

        rb->gravity = 2.0f;

        pi->rotation.x = 0;
        fe_quat_t rq = fe_quat_from_euler(&pi->rotation);

        fe_vec3_t front = fe_quat_mul_vec3(&rq, &FE_VEC3_FORWARD);
        fe_vec3_norm(&front);

        fe_vec3_t right = fe_quat_mul_vec3(&rq, &FE_VEC3_RIGHT);
        fe_vec3_norm(&right);

        const f32 ground_speed     = 0.08f;
        const f32 ground_speed_max = 1.0f;
        const f32 air_speed        = 0.05f;
        const f32 air_speed_max    = 0.45f;
        const f32 jump_speed       = 0.30f;

        fe_vec3_t trl = FE_VEC3_ZERO;
        if (pi->flags & CO_PLAYER_INPUT_FORWARD)  fe_vec3_add(&trl, &front);
        if (pi->flags & CO_PLAYER_INPUT_BACKWARD) fe_vec3_sub(&trl, &front);
        if (pi->flags & CO_PLAYER_INPUT_RIGHT)    fe_vec3_add(&trl, &right);
        if (pi->flags & CO_PLAYER_INPUT_LEFT)     fe_vec3_sub(&trl, &right);

        trl.z = 0;

        pi->jump_timer--;
        if (pi->jump_timer <= 0) pi->jump_timer = 0;

        bool grounded = fe_rigidbody_is_grounded(rb);

        if (can_jump(pi) && grounded && pi->jump_timer <= 0) 
        {
            trl.z            = 1;
            pi->jump_timer   = 2;
            //consider us airborne on this exact frame
            grounded = false;
        }

        fe_vec3_norm(&trl);

        if (grounded)
        {
            rb->friction = 0.85;
            accelerate(rb, &trl, ground_speed, ground_speed_max);
        }
        else
        {
            rb->friction = 1.0f;
            accelerate(rb, &trl, air_speed, air_speed_max);
        }

        if (!grounded && pi->jump_timer == 2)
        {
            fe_rigidbody_add_velocity(rb, &fe_vec3(0, 0, jump_speed));
        }
        
        pi->prev_flags = pi->flags;
        pi->flags = 0;
        
    }
}

