#pragma once

#include <foam/core/data/mesh.h>
#include <foam/core/math/math.h>

typedef struct
{
    fe_vec3_t min, max;
} fe_aabb_t;

#define fe_aabb(min, max) (fe_aabb_t){min, max}

void      fe_aabb_init(fe_aabb_t *aabb, const fe_vec3_t *min, const fe_vec3_t *max);
void      fe_aabb_init_in_world_space(fe_aabb_t *aabb, const fe_vec3_t *min, const fe_vec3_t *max, const fe_vec3_t *pos);
void      fe_aabb_init_in_world_space_from_center_and_size(fe_aabb_t *aabb, const fe_vec3_t *center, const fe_vec3_t *size, const fe_vec3_t *pos);
void      fe_aabb_init_from_mesh(fe_aabb_t *aabb, const fe_mesh_t *mesh);
void      fe_aabb_init_from_center_and_size(fe_aabb_t *aabb, const fe_vec3_t *center, const fe_vec3_t *size);
bool      fe_aabb_intersects_aabb(const fe_aabb_t *a, const fe_aabb_t *b);
bool      fe_aabb_contains_point(const fe_aabb_t *aabb, const fe_vec3_t *point);
fe_vec3_t fe_aabb_get_size(const fe_aabb_t *aabb);
fe_vec3_t fe_aabb_get_extents(const fe_aabb_t *aabb);
void      fe_aabb_translate(fe_aabb_t *aabb, const fe_vec3_t *trl);
void      fe_aabb_scale(fe_aabb_t *aabb, const fe_vec3_t *scale);
