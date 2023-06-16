#pragma once

#include <foam/core/ecs/ecs.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/genvec.h>
#include <foam/core/math/math.h>
#include <foam/core/physics/collision.h>
#include <foam/core/data/mesh_bundle.h>

typedef void (*fe_simple_collision_fn)       (fe_entity_id col, fe_entity_id other);
typedef void (*fe_simple_collision_enter_fn) (fe_entity_id col, fe_entity_id other);
typedef void (*fe_simple_collision_exit_fn)  (fe_entity_id col, fe_entity_id other);

typedef struct
{
    fe_aabb_t aabb;

    fe_vec(fe_entity_id)         collisions;
    fe_vec(fe_entity_id)         prev_collisions;
    fe_simple_collision_fn       collision_fn;
    fe_simple_collision_enter_fn collision_enter_fn;
    fe_simple_collision_exit_fn  collision_exit_fn;
} fe_simple_collider_t;


void fe_simple_collider_make(fe_simple_collider_t         *dst,
                             const fe_aabb_t              *aabb,
                             fe_simple_collision_enter_fn enter_fn,
                             fe_simple_collision_exit_fn  exit_fn,
                             fe_simple_collision_fn       collision_fn);

void fe_simple_collider_kill(fe_simple_collider_t *col);


