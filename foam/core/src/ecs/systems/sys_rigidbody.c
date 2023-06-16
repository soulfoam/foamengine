#include <foam/core/physics/physics.h>
#include <foam/core/ecs/ecs_info.h>

static void 
rigidbody_check_grounded(fe_rigidbody_t *rb)
{
    if (!rb->collision.found_collision) return;

    fe_vec3_t axis = FE_VEC3_ZERO;
    axis.z = 1.0f;

    fe_vec3_t a, b, c;
    fe_vec3_mul_to(&a, &rb->collision.a, &rb->radius);
    fe_vec3_mul_to(&b, &rb->collision.b, &rb->radius);
    fe_vec3_mul_to(&c, &rb->collision.c, &rb->radius);
    fe_plane_t plane;
    fe_plane_init_from_triangle(&plane, &a, &b, &c);

    rb->slope_angle = fe_vec3_dot(&plane.normal, &axis);

    if (rb->slope_angle >= fe_physics_get_slope_start_slide_angle()) 
    {
        rb->grounded = true;
    }
}

static void
rigidbody_try_triangle_collision(fe_rigidbody_t *rb, fe_mesh_collider_t *mc)
{
    const fe_mesh_t *mesh = &mc->data;

    fe_vec3_t a, b, c;
    for (u64 k = 0; k * 3 < fe_vec_get_count(mesh->indices); k++)
    {
        fe_vec3_div_to(&a, &mesh->verts[mesh->indices[3 * k]].pos,     &rb->collision.e_radius);
        fe_vec3_div_to(&b, &mesh->verts[mesh->indices[3 * k + 1]].pos, &rb->collision.e_radius);
        fe_vec3_div_to(&c, &mesh->verts[mesh->indices[3 * k + 2]].pos, &rb->collision.e_radius);
        fe_collision_check_triangle(&rb->collision, &a, &b, &c); 
    }
}


static void
rigidbody_try_collision(fe_rigidbody_t *rb)
{
    fe_ecs_query(FE_QUERY_MESH_COLLIDERS, e)
    {
        fe_mesh_collider_t *mc   = fe_entity_get_component(*e, FE_COMPONENT_MESH_COLLIDER);
        
        //rb aabb already is in world space and with scale applied, just need to figure out difference
        //between this frame and last to prevent going into meshes or passing through them
        fe_vec3_t rb_min, rb_max;
        fe_vec3_min_to(&rb_min, &rb->prev_aabb.min, &rb->aabb.min);
        fe_vec3_max_to(&rb_max, &rb->prev_aabb.max, &rb->aabb.max);
        fe_aabb_t rb_aabb;
        fe_aabb_init(&rb_aabb, &rb_min, &rb_max);
        fe_vec3_t scale = fe_aabb_get_size(&rb_aabb);
        fe_vec3_scale(&scale, 3);
        fe_aabb_scale(&rb_aabb, &scale);

        if (fe_aabb_intersects_aabb(&mc->aabb, &rb_aabb))
        {
            rigidbody_try_triangle_collision(rb, mc);
        }

    }

}

static void 
rigidbody_collide_with_world(fe_rigidbody_t *rb, fe_vec3_t *e_position, fe_vec3_t *e_velocity)
{
    fe_plane_t first_plane;
    fe_vec3_t dest;
    fe_vec3_add_to(&dest, e_position, e_velocity);

    fe_vec3_t tmp;
    for (u32 i = 0; i < 3; i++) 
    {
        fe_vec3_norm_to(&tmp, e_velocity);
        rb->collision.e_norm_velocity  = tmp;
        rb->collision.e_velocity       = *e_velocity;
        rb->collision.e_base_point     = *e_position;
        rb->collision.e_radius         = rb->radius;
        rb->collision.found_collision  = false;
        rb->collision.nearest_distance = FLT_MAX;
        rb->collision.t                = 0.0f;

        rigidbody_try_collision(rb);
        rigidbody_check_grounded(rb);

        if (!rb->collision.found_collision) 
        {
            *e_position = dest;
            return;
        }

        const f32 VERY_CLOSE_DIST = fe_physics_get_unit_scale() * 0.01f; 

        fe_vec3_t touch_point;
        fe_vec3_scale_to(&tmp, e_velocity, rb->collision.t);
        fe_vec3_add_to(&touch_point, e_position, &tmp);

        f32 dist       = fe_vec3_len(e_velocity) * rb->collision.t;
        f32 short_dist = MAX(dist - VERY_CLOSE_DIST, 0.0f);

        fe_vec3_norm_to(&tmp, e_velocity);
        fe_vec3_scale(&tmp, short_dist);
        fe_vec3_add(e_position, &tmp);

        fe_vec3_t slide_plane_origin;
        slide_plane_origin = rb->collision.intersect_point;

        fe_vec3_t slide_plane_normal;
        fe_vec3_sub_to(&slide_plane_normal, &touch_point, &rb->collision.intersect_point);
        fe_vec3_norm(&slide_plane_normal);

        if (i == 0) 
        {
            f32 long_radius = 1.0 + VERY_CLOSE_DIST;

            fe_plane_init(&first_plane, &slide_plane_origin, &slide_plane_normal);

            f32 dist_to_plane  = fe_plane_signed_distance(&first_plane, &dest) - long_radius;

            fe_vec3_scale_to(&tmp, &first_plane.normal, dist_to_plane);
            fe_vec3_sub(&dest, &tmp);

            fe_vec3_sub_to(e_velocity, &dest, e_position);

        } 
        else if (i == 1) 
        {
            fe_plane_t second_plane;
            fe_plane_init(&second_plane, &slide_plane_origin, &slide_plane_normal);

            fe_vec3_t crease;
            fe_vec3_cross_to(&crease, &first_plane.normal, &second_plane.normal);

            fe_vec3_sub_to(&tmp, &dest, e_position);
            f32 dis = fe_vec3_dot(&tmp, &crease);
            fe_vec3_norm(&crease);

            fe_vec3_scale_to(e_velocity, &crease, dis);
            fe_vec3_add_to(&dest, e_position, e_velocity);
        }
    }

    *e_position = dest;
}

static void 
rigidbody_collide_and_slide(fe_rigidbody_t *rb, fe_transform_t *xform)
{
    rb->collision.r3_position = xform->trl;
    rb->collision.r3_velocity = rb->velocity;
    rb->collision.e_radius    = rb->radius;

    fe_vec3_t gravity           = FE_VEC3_ZERO;
    gravity.z                   = rb->collision.r3_velocity.z;
    rb->collision.r3_velocity.z = 0.0f;

    fe_vec3_t e_position, e_velocity;
    fe_vec3_div_to(&e_position, &rb->collision.r3_position, &rb->collision.e_radius);
    fe_vec3_div_to(&e_velocity, &rb->collision.r3_velocity, &rb->collision.e_radius);

    //fe_log("slope angle [%f]", rb->slope_angle);
    //if (rb->slope_angle < SLOPE_UNWALKABLE_ANGLE) e_velocity[VEC3_UP_AXIS] += GRAVITY_FORCE;

    // velocity pass
    rigidbody_collide_with_world(rb, &e_position, &e_velocity); 

    // gravity pass
    rb->collision.r3_velocity = gravity;
    fe_vec3_div_to(&e_velocity, &gravity, &rb->collision.e_radius);

    rigidbody_collide_with_world(rb, &e_position, &e_velocity);

    xform->flags |= FE_TRANSFORM_DIRTY_LOCAL;
    fe_vec3_mul_to(&xform->trl, &e_position, &rb->collision.e_radius);

}


static void
rigidbody_fixed_tick(fe_rigidbody_t *rb, fe_transform_t *xform)
{
    rb->prev_aabb = rb->aabb;
    fe_aabb_init_from_center_and_size(&rb->aabb, fe_transform_get_world_translation(xform), 
                                      &fe_vec3(rb->radius.x , rb->radius.y, rb->radius.z));

    rb->velocity.z -= ((fe_physics_get_gravity() * rb->gravity) * fe_physics_get_unit_scale());

    fe_vec2_t xy = FE_VEC2_ZERO;
    xy.x = rb->velocity.x;
    xy.y = rb->velocity.y;

    // stop sliding on slopes
    if (rb->grounded && fabs(fe_vec2_len(&xy)) < 0.01f && rb->velocity.z <= 0.0f)
    {
        rb->velocity.z = 0.0f;
    }
    else
    {
        rb->grounded = false;
    }

    for (u32 i = 0; i < 5; i++)
    {
        rigidbody_collide_and_slide(rb, xform);
    }
    
    fe_vec3_sub_to(&rb->velocity, &xform->trl, &rb->collision.r3_position);
    f32 gravity = rb->velocity.z;
    fe_vec3_scale(&rb->velocity, rb->friction);
    rb->velocity.z = gravity;
}

void
fe_core_system_rigidbody_handle_collisions(void)
{
    fe_ecs_query(FE_QUERY_RIGIDBODIES, e)
    {
        fe_rigidbody_t *rb    = fe_entity_get_component(*e, FE_COMPONENT_RIGIDBODY);
        fe_transform_t *xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);
        rigidbody_fixed_tick(rb, xform);
    }
}

