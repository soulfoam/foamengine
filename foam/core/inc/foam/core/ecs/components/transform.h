#pragma once

#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/physics/ray.h>

typedef enum
{
    FE_TRANSFORM_DIRTY_LOCAL       = 1 << 0,
    FE_TRANSFORM_IGNORE_PARENT_TRL = 1 << 1,
    FE_TRANSFORM_IGNORE_PARENT_ROT = 1 << 2,
    FE_TRANSFORM_IGNORE_PARENT_SCL = 1 << 3,

} FE_TRANSFORM_FLAG;

typedef struct
{
    fe_vec3_t trl;
    fe_quat_t rot;
    fe_vec3_t scl;

    fe_vec3_t world_trl;
    fe_quat_t world_rot;
    fe_vec3_t world_scl;

    fe_mat4x4_t world_matrix;

    u8 flags;
} fe_transform_t;


void fe_transform_init(fe_transform_t *xform);
void fe_transform_init_t(fe_transform_t *xform, const fe_vec3_t *trl);
void fe_transform_init_tr(fe_transform_t *xform, const fe_vec3_t *trl, const fe_vec3_t *rot);
void fe_transform_init_trs(fe_transform_t *xform, const fe_vec3_t *trl, const fe_vec3_t *rot, const fe_vec3_t *scl);
void fe_transform_translate(fe_transform_t *xform, const fe_vec3_t *trl);
void fe_transform_rotate_euler(fe_transform_t *xform, const fe_vec3_t *rot);
void fe_transform_rotate_at_point(fe_transform_t *xform, const fe_quat_t *rot, const fe_vec3_t *point);
void fe_transform_rotate(fe_transform_t *xform, const fe_quat_t *rot);
void fe_transform_rotate_mul(fe_transform_t *xform, const fe_quat_t *rot);
void fe_transform_look_at(fe_transform_t *xform, const fe_vec3_t *targ);
void fe_transform_scale(fe_transform_t *xform, const fe_vec3_t *scl);
void fe_transform_generate_matrix(const fe_transform_t *xform, fe_mat4x4_t *dst);
void fe_transform_generate_world_matrix(const fe_transform_t *xform, fe_mat4x4_t *dst);
void fe_transform_generate_view_matrix(const fe_transform_t *xform, fe_mat4x4_t *dst);
void fe_transform_generate_world_view_matrix(const fe_transform_t *xform, fe_mat4x4_t *dst);
const fe_vec3_t*   fe_transform_get_translation(const fe_transform_t *xform);
const fe_quat_t*   fe_transform_get_rotation(const fe_transform_t *xform);
const fe_vec3_t*   fe_transform_get_scale(const fe_transform_t *xform);
const fe_vec3_t*   fe_transform_get_world_translation(const fe_transform_t *xform);
const fe_quat_t*   fe_transform_get_world_rotation(const fe_transform_t *xform);
const fe_vec3_t*   fe_transform_get_world_scale(const fe_transform_t *xform);

void fe_transform_set_translation(fe_transform_t *xform, const fe_vec3_t *trl);
void fe_transform_set_rotation(fe_transform_t *xform, const fe_quat_t *rot);
void fe_transform_set_scale(fe_transform_t *xform, const fe_vec3_t *scl);

void fe_transform_get_forward(const fe_transform_t *xform, fe_vec3_t *dst);
void fe_transform_get_right(const fe_transform_t *xform, fe_vec3_t *dst);
void fe_transform_get_up(const fe_transform_t *xform, fe_vec3_t *dst);
void fe_transform_get_world_forward(const fe_transform_t *xform, fe_vec3_t *dst);
void fe_transform_get_world_right(const fe_transform_t *xform, fe_vec3_t *dst);
void fe_transform_get_world_up(const fe_transform_t *xform, fe_vec3_t *dst);
void fe_transform_get_ray(const fe_transform_t *xform, fe_ray_t *dst);
void fe_transform_get_world_ray(const fe_transform_t *xform, fe_ray_t *dst);
void fe_transform_print(const fe_transform_t *xform);

