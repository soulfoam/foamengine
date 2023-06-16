#pragma once

#include <foam/core/ecs/ecs.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/genvec.h>
#include <foam/core/math/math.h>
#include <foam/core/physics/collision.h>
#include <foam/core/data/mesh_bundle.h>

typedef struct
{
    const fe_mesh_t *mesh;
    fe_mesh_t data;

    //keep this in world space
    fe_aabb_t aabb;

} fe_mesh_collider_t;

void fe_mesh_collider_make(fe_mesh_collider_t *mc, const fe_mesh_t *mesh);
void fe_mesh_collider_kill(fe_mesh_collider_t *mc);
void fe_mesh_collider_recalculate(fe_mesh_collider_t *mc, const fe_vec3_t *trl, const fe_quat_t *rot, const fe_vec3_t *scl);

