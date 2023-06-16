#include <foam/core/ext/logger.h>
#include <foam/core/ext/hashmap.h>
#include <foam/core/physics/physics.h>

#define UP_AXIS 2

typedef struct
{
    f32 gravity;

    f32 units_per_meter;
    f32 slope_start_slide_angle;

} fe_physics_t;

static fe_physics_t physics;

//static bool
//contains_collision(fe_vec(fe_collider_id) collisions, fe_collider_id id)
//{
//    if (!collisions) return false;
//
//    for (u64 i = 0; i < fe_vec_get_count(collisions); i++)
//    {
//        if (collisions[i] == id) return true;
//    }
//
//    return false;
//}

static void
update_colliders(void)
{
  //  fe_genvec_for_each(physics.colliders, col)
  //  {
  //      fe_vec_clear(col->prev_collisions); 
  //      fe_vec_append(col->prev_collisions, col->collisions);
  //      fe_vec_clear(col->collisions); 
  //  }
}

static void
handle_sphere_callbacks(void)
{
  //  fe_genvec_for_each(physics.colliders, col)
  //  {
  //      fe_collider_id col_id = fe_genvec_for_each_cur_id(physics.colliders);
  //      for (u64 i = 0; i < fe_vec_get_count(col->prev_collisions); i++)
  //      {
  //          fe_collider_id prev_id = col->prev_collisions[i];
  //          if (!contains_collision(col->collisions, prev_id))
  //          {
  //              if (col->collision_exit_fn) col->collision_exit_fn(col_id, prev_id);
  //          }
  //      }

  //      for (u64 i = 0; i < fe_vec_get_count(col->collisions); i++)
  //      {
  //          fe_collider_id id = col->collisions[i];
  //          if (!contains_collision(col->prev_collisions, id))
  //          {
  //              if (col->collision_enter_fn) col->collision_enter_fn(col_id, id);
  //          }
  //          else
  //          {
  //              if (col->collision_fn) col->collision_fn(col_id, id);
  //          }
  //      }

  //  }
}

static void
gather_sphere_collisions(void)
{
  //  for (u32 i = 0; i < fe_genvec_capacity(physics.colliders); i++)
  //  {
  //      if (!fe_genvec_valid_index(physics.colliders, i)) continue; 
  //      fe_collider_t *c0 = &physics.colliders[i];
  //      if (c0->mesh != NULL) continue;

  //      for (u32 j = i+1; j < fe_genvec_capacity(physics.colliders); j++)
  //      {    
  //          if (!fe_genvec_valid_index(physics.colliders, j)) continue;
  //          fe_collider_t *c1 = &physics.colliders[j];
  //          if (c1->mesh != NULL) continue;
  //          
  //          if (fe_collider_sphere_to_sphere(c0, c1))
  //          {
  //              fe_vec_add(c0->collisions, FE_GENVEC_ID_AT(physics.colliders, j));
  //              fe_vec_add(c1->collisions, FE_GENVEC_ID_AT(physics.colliders, i));
  //          }
  //      }
  //  }
}

static void
update_rigidbodies(void)
{
    //fe_genvec_for_each(physics.rigidbodies, rb)
    //{
    //    fe_rigidbody_tick(rb); 
    //}
}

void 
fe_physics_setup(void)
{
    physics.gravity = 2.0f;

    physics.units_per_meter         = 1.0f;
    physics.slope_start_slide_angle = 0.45f;
}

void 
fe_physics_cleanup(void)
{

}

void
fe_physics_tick(void)
{
    gather_sphere_collisions();
    handle_sphere_callbacks();
    update_colliders(); 
    update_rigidbodies();
}

void
fe_physics_try_rigidbody_collision(fe_rigidbody_t *rb)
{
  //  vec3 rb_center = 
  //  {
  //      (rb->position[0] + (rb->position[0] + rb->radius[0])) / 2, 
  //      (rb->position[1] + (rb->position[1] + rb->radius[1])) / 2,
  //      (rb->position[2] + (rb->position[2] + rb->radius[2])) / 2
  //  };

  //  f32 rb_radius = rb->radius[0];
  //  for (u32 i = 1; i < 3; i++) rb_radius = MAX(rb_radius, rb->radius[i]);

  //  fe_sphere_t rb_sphere = { VEC3_DUP(rb_center), rb_radius };

  //  fe_genvec_for_each(physics.colliders, col)
  //  {
  //      if (col->mesh == NULL) continue;

  //      fe_sphere_t col_sphere = { VEC3_ADD(col->sphere.center, col->pos), col->sphere.radius };

  //      if (fe_sphere_to_sphere(&rb_sphere, &col_sphere))
  //      {
  //          try_collision(&rb->collision, col);
  //      }
  //  }
}

//static f32 
//raycast_sphere(const fe_sphere_t *sphere, const fe_vec3_t *from, const fe_vec3_t *dir)
//{
//    fe_vec3_t oc;
//    fe_vec3_sub_to(&oc, from, &sphere->center);
//    f32 a = fe_vec3_dot(dir, dir);
//    f32 b = 2.0 * fe_vec3_dot(&oc, dir);
//    f32 c = fe_vec3_dot(&oc, &oc) - sphere->radius * sphere->radius;
//    f32 discriminant = b*b - 4*a*c;
//    if(discriminant < 0.0)
//    {
//        return -1.0;
//    }
//    else
//    {
//        float numerator = -b - sqrt(discriminant);
//        if (numerator > 0.0) return numerator / (2.0 * a);
//
//        numerator = -b + sqrt(discriminant);
//
//        if (numerator > 0.0) return numerator / (2.0 * a);
//        else                 return -1;
//    }
//}

//f32 
//fe_physics_raycast_sphere(fe_raycast_sphere_hit_t *dst, const vec3 from, const vec3 dir)
//{
    //fe_genvec_for_each(physics.colliders, col)
    //{
    //    f32 r = raycast_sphere(&col->sphere, from, dir);
    //    if (r != -1) return r;
    //}    

    //return -1;
//}

//bool 
//fe_physics_raycast_mesh(fe_raycast_mesh_hit_t *dst, const vec3 from, const vec3 dir)
//{
    //fe_genvec_for_each(physics.colliders, col)
    //{        
    //    vec3 col_min;
    //    vec3_add(col_min, col->sphere.min, col->pos);
    //    vec3 col_max;
    //    vec3_add(col_max, col->sphere.max, col->pos);
    //    f32 t1 = (col_min[0] - from[0]) * dir[0];
    //    f32 t2 = (col_max[0] - from[0]) * dir[0];
    //    f32 t3 = (col_min[1] - from[1]) * dir[1];
    //    f32 t4 = (col_max[1] - from[1]) * dir[1];
    //    f32 t5 = (col_min[2] - from[2]) * dir[2];
    //    f32 t6 = (col_max[2] - from[2]) * dir[2];

    //    f32 tmin = MAX(MAX(MIN(t1, t2), MIN(t3, t4)), MIN(t5, t6));
    //    f32 tmax = MIN(MIN(MAX(t1, t2), MAX(t3, t4)), MAX(t5, t6));

    //    // if tmax < 0, ray (line) is intersecting sphere, but the whole sphere is behind us
    //    if (tmax < 0)
    //    {
    //        dst->ray_len = tmax;
    //        continue;
    //    }

    //    // if tmin > tmax, ray doesn't intersect sphere
    //    if (tmin > tmax)
    //    {
    //        dst->ray_len = tmax;
    //        continue;
    //    }

    //    dst->id      = fe_genvec_for_each_cur_id(physics.colliders);
    //    dst->ray_len = tmin;
    //    return true;
    //}

    //return false;
//}

void           
fe_physics_set_gravity(f32 gravity)
{
    physics.gravity = gravity;
}

f32           
fe_physics_get_gravity(void)
{
    return physics.gravity;
}

void 
fe_physics_set_slope_start_slide_angle(f32 angle)
{
    physics.slope_start_slide_angle = angle;
}

f32 
fe_physics_get_slope_start_slide_angle(void)
{
    return physics.slope_start_slide_angle;
}

void           
fe_physics_set_units_per_meter(f32 upm)
{
    physics.units_per_meter = upm;
}

f32           
fe_physics_get_unit_scale(void)
{
    return (physics.units_per_meter / 100.0f);
}

