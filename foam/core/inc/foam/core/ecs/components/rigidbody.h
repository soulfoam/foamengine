#pragma once

#include <foam/core/math/math.h>
#include <foam/core/physics/collision.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/data/mesh_bundle.h>
#include <foam/core/ext/genvec.h>

typedef fe_genvec_id fe_rigidbody_id;

typedef struct
{
    fe_vec3_t      radius;
    f32            friction;
    f32            gravity;

    fe_vec3_t      velocity;
    bool           grounded;
    f32            slope_angle;
    fe_collision_t collision;

    fe_aabb_t      aabb, prev_aabb; //broadphase

} fe_rigidbody_t;

void fe_rigidbody_init(fe_rigidbody_t *rb, const fe_vec3_t *radius);
void fe_rigidbody_add_velocity(fe_rigidbody_t *rb, const fe_vec3_t *vel);

void fe_rigidbody_set_velocity(fe_rigidbody_t *rb, const fe_vec3_t *vel);
void fe_rigidbody_set_radius(fe_rigidbody_t *rb, const fe_vec3_t *radius);
void fe_rigidbody_set_friction(fe_rigidbody_t *rb, f32 friction);
void fe_rigidbody_set_gravity(fe_rigidbody_t *rb, f32 gravity);

fe_vec3_t fe_rigidbody_get_velocity(const fe_rigidbody_t *rb);
fe_vec3_t fe_rigidbody_get_radius(const fe_rigidbody_t *rb);
f32  fe_rigidbody_get_friction(const fe_rigidbody_t *rb);
f32  fe_rigidbody_get_gravity(const fe_rigidbody_t *rb);
bool fe_rigidbody_is_grounded(const fe_rigidbody_t *rb);
f32  fe_rigidbody_get_slope_angle(const fe_rigidbody_t *rb);
