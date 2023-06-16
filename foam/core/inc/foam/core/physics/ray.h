#pragma once

#include <foam/core/physics/aabb.h>
#include <foam/core/ecs/ecs.h>

typedef struct
{
    fe_vec3_t orig;
    fe_vec3_t dir;
} fe_ray_t;

#define fe_ray(orig, dir) (fe_ray_t){orig, dir}

bool         fe_ray_vs_aabb(const fe_ray_t *ray, const fe_aabb_t *aabb, f32 *dist_dst);
bool         fe_ray_vs_triangle(const fe_ray_t *ray, fe_vec3_t *intersect_dst, f32 *t_dst, 
                   const fe_vec3_t *v0, const fe_vec3_t *v1, const fe_vec3_t *v2);
fe_entity_id fe_ray_vs_simple_colliders(const fe_ray_t *ray, f32 *dist);
fe_entity_id fe_ray_vs_mesh_collider_aabbs(const fe_ray_t *ray, f32 *dist);
