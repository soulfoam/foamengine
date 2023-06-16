#include <foam/core/ecs/components/simple_collider.h>


void 
fe_simple_collider_make(fe_simple_collider_t              *dst,
                             const fe_aabb_t              *aabb,
                             fe_simple_collision_enter_fn enter_fn,
                             fe_simple_collision_exit_fn  exit_fn,
                             fe_simple_collision_fn       collision_fn)
{
    dst->aabb = *aabb;
    dst->collisions         = FE_VEC_EMPTY;
    dst->prev_collisions    = FE_VEC_EMPTY;
    dst->collision_fn       = collision_fn;
    dst->collision_enter_fn = enter_fn;
    dst->collision_exit_fn  = exit_fn;
}

void
fe_simple_collider_kill(fe_simple_collider_t *col)
{
    fe_vec_kill(col->collisions, NULL);
    fe_vec_kill(col->prev_collisions, NULL);
}

