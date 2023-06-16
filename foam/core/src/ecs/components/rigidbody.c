#include <foam/core/ecs/components/rigidbody.h>
#include <foam/core/ext/logger.h>
#include <foam/core/physics/collision.h>
#include <foam/core/physics/physics.h>

static void
init_collision(fe_rigidbody_t *rb)
{
    fe_vec3_zero(&rb->collision.r3_velocity);
    fe_vec3_zero(&rb->collision.r3_position);
    fe_vec3_one(&rb->collision.e_radius);
    fe_vec3_zero(&rb->collision.e_velocity);
    fe_vec3_zero(&rb->collision.e_norm_velocity);
    fe_vec3_zero(&rb->collision.e_base_point);
    fe_vec3_zero(&rb->collision.a);
    fe_vec3_zero(&rb->collision.b);
    fe_vec3_zero(&rb->collision.c);
    rb->collision.found_collision = false;
    rb->collision.nearest_distance = 0;
    rb->collision.t = 0;
    fe_vec3_zero(&rb->collision.intersect_point);
    fe_plane_init(&rb->collision.plane, &fe_vec3(0, 0, 0), &fe_vec3(0, 0, 0));
    rb->collision.depth = 0;
}

void
fe_rigidbody_init(fe_rigidbody_t *rb, const fe_vec3_t *radius)
{
    fe_vec3_zero(&rb->velocity);
    rb->radius      = *radius;
    rb->grounded    = false;
    rb->slope_angle = 0.0f;
    rb->friction    = 0.60f;
    rb->gravity     = 1.0f;
    fe_aabb_init_from_center_and_size(&rb->aabb, &FE_VEC3_ZERO, radius);
    fe_aabb_init_from_center_and_size(&rb->prev_aabb, &FE_VEC3_ZERO, radius);

    init_collision(rb);
}

void
fe_rigidbody_add_velocity(fe_rigidbody_t *rb, const fe_vec3_t *vel)
{
    fe_vec3_add(&rb->velocity, vel);
}

void 
fe_rigidbody_set_velocity(fe_rigidbody_t *rb, const fe_vec3_t *vel)
{
    rb->velocity = *vel;
}

void 
fe_rigidbody_set_radius(fe_rigidbody_t *rb, const fe_vec3_t *radius)
{
    rb->radius = *radius;
}

void 
fe_rigidbody_set_friction(fe_rigidbody_t *rb, f32 friction)
{
    rb->friction = friction;
}

void 
fe_rigidbody_set_gravity(fe_rigidbody_t *rb, f32 gravity)
{
    rb->gravity = gravity;
}

fe_vec3_t 
fe_rigidbody_get_velocity(const fe_rigidbody_t *rb)
{
    return rb->velocity;
}

fe_vec3_t 
fe_rigidbody_get_radius(const fe_rigidbody_t *rb)
{
    return rb->radius;
}

f32  
fe_rigidbody_get_friction(const fe_rigidbody_t *rb)
{
    return rb->friction;
}

f32  
fe_rigidbody_get_gravity(const fe_rigidbody_t *rb)
{
    return rb->gravity;
}

bool 
fe_rigidbody_is_grounded(const fe_rigidbody_t *rb)
{
    return rb->grounded;
}

f32 
fe_rigidbody_get_slope_angle(const fe_rigidbody_t *rb)
{
    return rb->slope_angle;
}



