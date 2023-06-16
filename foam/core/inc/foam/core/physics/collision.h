#pragma once

#include <foam/core/math/math.h>
#include <foam/core/data/mesh.h>
#include <foam/core/physics/aabb.h>

typedef struct 
{
    fe_vec3_t origin;
    fe_vec3_t normal;
    fe_vec4_t equation;
} fe_plane_t;

typedef struct 
{
    // r3 space
    fe_vec3_t r3_velocity;
    fe_vec3_t r3_position;

    // ellipsoid space
    fe_vec3_t e_radius;
    fe_vec3_t e_velocity;
    fe_vec3_t e_norm_velocity;
    fe_vec3_t e_base_point;

    // original tri points
    fe_vec3_t a;
    fe_vec3_t b;
    fe_vec3_t c;

    // hit information
    bool found_collision;
    f32 nearest_distance;
    f64 t;
    fe_vec3_t intersect_point;
    fe_plane_t plane;

    // iteration depth
    u32 depth;
} fe_collision_t;


void        fe_plane_init(fe_plane_t *plane, const fe_vec3_t *origin, const fe_vec3_t *normal);
void        fe_plane_init_from_triangle(fe_plane_t *plane, const fe_vec3_t *a, const fe_vec3_t *b, const fe_vec3_t *c);
f32         fe_plane_signed_distance(const fe_plane_t *plane, const fe_vec3_t *base_point);
bool        fe_plane_is_front_facing(const fe_plane_t *plane, const fe_vec3_t *direction);
bool        fe_check_point_in_triangle(const fe_vec3_t *point, const fe_vec3_t *p1, const fe_vec3_t *p2, const fe_vec3_t *p3);
bool        fe_get_lowest_root(f32 a, f32 b, f32 c, f32 max, f32 *root);
void        fe_collision_check_triangle(fe_collision_t *collision, const fe_vec3_t *p1, const fe_vec3_t *p2, const fe_vec3_t *p3);

