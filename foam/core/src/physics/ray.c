#include <foam/core/physics/ray.h>
#include <foam/core/ecs/ecs_info.h>
#include <foam/core/ext/logger.h>

bool
fe_ray_vs_aabb(const fe_ray_t *ray, const fe_aabb_t *aabb, f32 *dist_dst)
{
    fe_vec3_t dirfrac;
    dirfrac.x = 1.0f / ray->dir.x;
    dirfrac.y = 1.0f / ray->dir.y;
    dirfrac.z = 1.0f / ray->dir.z;

    f32 t1 = (aabb->min.x - ray->orig.x)*dirfrac.x;
    f32 t2 = (aabb->max.x - ray->orig.x)*dirfrac.x;
    f32 t3 = (aabb->min.y - ray->orig.y)*dirfrac.y;
    f32 t4 = (aabb->max.y - ray->orig.y)*dirfrac.y;
    f32 t5 = (aabb->min.z - ray->orig.z)*dirfrac.z;
    f32 t6 = (aabb->max.z - ray->orig.z)*dirfrac.z;

    f32 tmin = fmax(fmax(fmin(t1, t2), fmin(t3, t4)), fmin(t5, t6));
    f32 tmax = fmin(fmin(fmax(t1, t2), fmax(t3, t4)), fmax(t5, t6));

    f32 t = 0;
    if (tmax < 0)
    {
        t = tmax;
        return false;
    }

    if (tmin > tmax)
    {
        t = tmax;
        return false;
    }

    t = tmin;
    
    if (dist_dst) *dist_dst = t;

    return true;
}


bool 
fe_ray_vs_triangle(const fe_ray_t *ray, fe_vec3_t *intersect_dst, f32 *t_dst, 
                   const fe_vec3_t *v0, const fe_vec3_t *v1, const fe_vec3_t *v2)
{
    fe_vec3_t vector;
    fe_vec3_norm_to(&vector, &ray->dir);

    fe_vec3_t edge1, edge2, h, s, q;
    fe_vec3_sub_to(&edge1, v1, v0);
    fe_vec3_sub_to(&edge2, v2, v0);

    fe_vec3_cross_to(&h, &vector, &edge2);
    f32 a = fe_vec3_dot(&edge1, &h);

    if (a > -FLT_EPSILON && a < FLT_EPSILON) return false;

    f32 f = 1.0 / a;

    fe_vec3_sub_to(&s, &ray->orig, v0);

    f32 u = f * fe_vec3_dot(&s, &h);
    if (u < 0.0 || u > 1.0) return false;

    fe_vec3_cross_to(&q, &s, &edge1);
    f32 v = f * fe_vec3_dot(&vector, &q);
    if (v < 0.0 || u + v > 1.0) return false;

    bool hit = false;
    f32 tmp_t = f * fe_vec3_dot(&edge2, &q);
    fe_vec3_t intersect_point = FE_VEC3_ZERO;
    if (tmp_t > FLT_EPSILON) 
    {
        fe_vec3_t tmp;
        fe_vec3_scale_to(&tmp, &vector, tmp_t);
        fe_vec3_add_to(&intersect_point, &ray->orig, &tmp);
        hit = true;
    }

    if (t_dst)         *t_dst = tmp_t;
    if (intersect_dst) *intersect_dst = intersect_point;

    return hit;
}

fe_entity_id
fe_ray_vs_simple_colliders(const fe_ray_t *ray, f32 *dist)
{
    fe_ecs_query(FE_QUERY_SIMPLE_COLLIDERS, e)
    {
        fe_simple_collider_t *sc = fe_entity_get_component(*e, FE_COMPONENT_SIMPLE_COLLIDER);
        fe_transform_t *sc_xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);
        fe_aabb_t aabb;
        fe_aabb_init_in_world_space(&aabb, &sc->aabb.min, &sc->aabb.max, fe_transform_get_world_translation(sc_xform));

        if (fe_ray_vs_aabb(ray, &aabb, dist))
        {
            return *e;
        }
    }
    
    return (fe_entity_id){FE_INVALID_ID};
}

fe_entity_id
fe_ray_vs_mesh_collider_aabbs(const fe_ray_t *ray, f32 *dist)
{
    fe_ecs_query(FE_QUERY_MESH_COLLIDERS, e)
    {
        fe_mesh_collider_t *mc = fe_entity_get_component(*e, FE_COMPONENT_MESH_COLLIDER);

        if (fe_ray_vs_aabb(ray, &mc->aabb, dist))
        {
            return *e;
        }
    }
    
    return (fe_entity_id){FE_INVALID_ID};
}
