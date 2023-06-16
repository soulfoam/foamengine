#pragma once

#include <foam/core/ext/sugar.h>
#include <foam/core/ext/genvec.h>
#include <foam/core/math/math.h>
#include <foam/core/ecs/components/rigidbody.h>
#include <foam/core/physics/collision.h>
#include <foam/core/data/mesh_bundle.h>

typedef struct
{
   // fe_collider_id id;
    f32            ray_len;
} fe_raycast_sphere_hit_t;

typedef struct
{
    //fe_collider_id id;
    //vec3           intersect_point;
    f32            t;
} fe_raycast_mesh_hit_t;

void fe_physics_setup(void);
void fe_physics_cleanup(void);

void fe_physics_set_slope_start_slide_angle(f32 angle);
f32  fe_physics_get_slope_start_slide_angle(void);
void fe_physics_set_units_per_meter(f32 upm);
f32  fe_physics_get_unit_scale(void);
void fe_physics_set_gravity(f32 gravity);
f32  fe_physics_get_gravity(void);


