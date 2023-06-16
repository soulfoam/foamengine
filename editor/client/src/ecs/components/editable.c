#include <client/ecs/components/editable.h>

void
ed_editable_init(ed_editable_t *editable)
{
    editable->aabb             = fe_aabb(FE_VEC3_ZERO, FE_VEC3_ZERO);
    editable->selected         = false;
    editable->mode             = 0;
    editable->mode_val         = FE_VEC3_ZERO;
    editable->use_axis         = FE_VEC3_ONE;
    editable->orig_pickup_trl  = FE_VEC3_ZERO;
    editable->dist_from_ray    = 0;
}

bool 
ed_editable_check_ray(ed_editable_t *editable, const fe_ray_t *ray, fe_vec3_t *intersect_dst, f32 *t)
{

}
