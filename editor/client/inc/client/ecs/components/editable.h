#pragma once

#include <foam/foam_client.h>

typedef enum
{
    EDITABLE_MODE_TRANSLATE = 1 << 0,
    EDITABLE_MODE_ROTATE    = 1 << 1,
    EDITABLE_MODE_SCALE     = 1 << 2,

} ED_EDITABLE_MODE;


typedef struct
{
    fe_aabb_t aabb;

    fe_mesh_t mesh;
    bool      has_mesh;

    bool      selected;
    u8        mode;
    fe_vec3_t mode_val;
    fe_vec3_t use_axis;

    f32       dist_from_ray;
    fe_vec3_t orig_pickup_trl;

} ed_editable_t;

void ed_editable_init(ed_editable_t *editable);
bool ed_editable_check_ray(ed_editable_t *editable, const fe_ray_t *ray, fe_vec3_t *intersect_dst, f32 *t);
