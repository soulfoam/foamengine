#include <foam/core/physics/aabb.h>

static void
aabb_verify_size(fe_aabb_t *aabb)
{
    fe_vec3_t size = fe_aabb_get_size(aabb);
    if (size.x <= 1)
    {
        aabb->min.x -= 0.5f;
        aabb->max.x += 0.5f;
    }
    if (size.y <= 1)
    {
        aabb->min.y -= 0.5f;
        aabb->max.y += 0.5f;
    }
    if (size.z <= 1)
    {
        aabb->min.z -= 0.5f;
        aabb->max.z += 0.5f;
    }
}

void
fe_aabb_init(fe_aabb_t *aabb, const fe_vec3_t *min, const fe_vec3_t *max)
{
    aabb->min = *min;
    aabb->max = *max;

    aabb_verify_size(aabb);

}

void
fe_aabb_init_in_world_space(fe_aabb_t *aabb, const fe_vec3_t *min, const fe_vec3_t *max, const fe_vec3_t *pos)
{
    fe_vec3_add_to(&aabb->min, min, pos);
    fe_vec3_add_to(&aabb->max, max, pos);

    aabb_verify_size(aabb);
}

void      
fe_aabb_init_in_world_space_from_center_and_size(fe_aabb_t *aabb, const fe_vec3_t *center, const fe_vec3_t *size, const fe_vec3_t *pos)
{
    fe_vec3_t half_size = *size;
    fe_vec3_mul(&half_size, &fe_vec3(0.5f, 0.5f, 0.5f));
    fe_vec3_sub_to(&aabb->min, center, &half_size);
    fe_vec3_add_to(&aabb->max, center, &half_size);
 
    fe_vec3_add(&aabb->min, pos);
    fe_vec3_add(&aabb->max, pos);

    aabb_verify_size(aabb);
}

void
fe_aabb_init_from_mesh(fe_aabb_t *aabb, const fe_mesh_t *mesh)
{
    fe_vec3_t min, max;
    min = mesh->verts[0].pos;
    max = mesh->verts[0].pos;

    fe_vec_for_each(mesh->verts, v)
    {
        fe_vec3_min_to(&min, &v->pos, &min);
        fe_vec3_max_to(&max, &v->pos, &max);
    }

    aabb->min = min;
    aabb->max = max;

    aabb_verify_size(aabb);
}

void
fe_aabb_init_from_center_and_size(fe_aabb_t *aabb, const fe_vec3_t *center, const fe_vec3_t *size)
{
    fe_vec3_t half_size = *size;
    fe_vec3_mul(&half_size, &fe_vec3(0.5f, 0.5f, 0.5f));
    fe_vec3_sub_to(&aabb->min, center, &half_size);
    fe_vec3_add_to(&aabb->max, center, &half_size);

    aabb_verify_size(aabb);
}

bool
fe_aabb_intersects_aabb(const fe_aabb_t *a, const fe_aabb_t *b)
{
     return
     (a->min.x <= b->max.x && a->max.x >= b->min.x) &&
     (a->min.y <= b->max.y && a->max.y >= b->min.y) &&
     (a->min.z <= b->max.z && a->max.z >= b->min.z);
}

bool
fe_aabb_contains_point(const fe_aabb_t *aabb, const fe_vec3_t *point)
{
    return
    (point->x >= aabb->min.x && point->x <= aabb->max.x) &&
    (point->y >= aabb->min.y && point->y <= aabb->max.y) &&
    (point->z >= aabb->min.z && point->z <= aabb->max.z);
}

fe_vec3_t
fe_aabb_get_size(const fe_aabb_t *aabb)
{
    fe_vec3_t ret;
    fe_vec3_sub_to(&ret, &aabb->max, &aabb->min);
    return ret;
}

fe_vec3_t
fe_aabb_get_extents(const fe_aabb_t *aabb)
{
    fe_vec3_t ret = fe_aabb_get_size(aabb);
    fe_vec3_scale(&ret, 0.5f);
    return ret;
}

void      
fe_aabb_translate(fe_aabb_t *aabb, const fe_vec3_t *trl)
{
    fe_vec3_add(&aabb->min, trl);
    fe_vec3_add(&aabb->max, trl);
}

void
fe_aabb_scale(fe_aabb_t *aabb, const fe_vec3_t *scale)
{
    fe_vec3_t half_scale = fe_vec3(scale->x / 2, scale->y / 2, scale->z / 2);
    fe_vec3_sub(&aabb->min, &half_scale);
    fe_vec3_add(&aabb->max, &half_scale);
}


