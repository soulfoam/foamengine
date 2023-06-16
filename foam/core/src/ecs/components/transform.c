#include <foam/core/core.h>
#include <foam/core/ecs/components/transform.h>
#include <foam/core/ext/logger.h>

static void
init(fe_transform_t *xform, const fe_vec3_t *trl, const fe_vec3_t *rot, const fe_vec3_t *scl)
{
    xform->trl = *trl;
    xform->rot = fe_quat_from_euler(rot);
    xform->scl = *scl;

    xform->world_trl = xform->trl;
    xform->world_rot = xform->rot;
    xform->world_scl = xform->scl;
  
    fe_mat4x4_init(&xform->world_matrix, &xform->trl, &xform->rot, &xform->scl);

    xform->flags = 0;
    xform->flags |= FE_TRANSFORM_DIRTY_LOCAL;

}

void
fe_transform_init(fe_transform_t *xform)
{
    init(xform, &FE_VEC3_ZERO, &FE_VEC3_ZERO, &FE_VEC3_ONE);
}

void
fe_transform_init_t(fe_transform_t *xform, const fe_vec3_t *trl)
{
    init(xform, trl, &FE_VEC3_ZERO, &FE_VEC3_ONE);
}

void
fe_transform_init_tr(fe_transform_t *xform, const fe_vec3_t *trl, const fe_vec3_t *rot)
{
    init(xform, trl, rot, &FE_VEC3_ONE);
}

void
fe_transform_init_trs(fe_transform_t *xform, const fe_vec3_t *trl, const fe_vec3_t *rot, const fe_vec3_t *scl)
{
    init(xform, trl, rot, scl);
}

void 
fe_transform_translate(fe_transform_t *xform, const fe_vec3_t *trl)
{ 
    fe_vec3_add(&xform->trl, trl);
    xform->flags |= FE_TRANSFORM_DIRTY_LOCAL;
}

void 
fe_transform_rotate_euler(fe_transform_t *xform, const fe_vec3_t *rot)
{ 
    xform->rot = fe_quat_from_euler(rot);
    xform->flags |= FE_TRANSFORM_DIRTY_LOCAL;
}

void 
fe_transform_rotate_at_point(fe_transform_t *xform, const fe_quat_t *rot, const fe_vec3_t *point)
{ 
    fe_vec3_t cur_trl = xform->trl;
    fe_transform_set_translation(xform, point);
    fe_transform_rotate(xform, rot);
    fe_transform_set_translation(xform, &cur_trl);
}

void 
fe_transform_rotate(fe_transform_t *xform, const fe_quat_t *rot)
{ 
    xform->rot = *rot;
    xform->flags |= FE_TRANSFORM_DIRTY_LOCAL;
}

void 
fe_transform_rotate_mul(fe_transform_t *xform, const fe_quat_t *rot)
{ 
    fe_quat_t q = xform->rot;
    fe_quat_mul_to(&xform->rot, rot, &q);
    xform->flags |= FE_TRANSFORM_DIRTY_LOCAL;
}

void
fe_transform_look_at(fe_transform_t *xform, const fe_vec3_t *targ)
{
    fe_vec3_t to;
    fe_vec3_sub_to(&to, targ, &xform->trl);
    fe_vec3_norm(&to);
    fe_mat4x4_t mat = fe_mat4x4_look_at(&xform->trl, &to, &FE_VEC3_UP);
    xform->rot = fe_quat_from_mat4x4(&mat);
    xform->flags |= FE_TRANSFORM_DIRTY_LOCAL;
}

void 
fe_transform_scale(fe_transform_t *xform, const fe_vec3_t *scl)
{
    xform->scl = *scl;
    xform->flags |= FE_TRANSFORM_DIRTY_LOCAL;
}

void
fe_transform_generate_matrix(const fe_transform_t *xform, fe_mat4x4_t *dst)
{
    fe_mat4x4_init(dst, &xform->trl, &xform->rot, &xform->scl);
}

void
fe_transform_generate_world_matrix(const fe_transform_t *xform, fe_mat4x4_t *dst)
{
    fe_mat4x4_init(dst, &xform->world_trl, &xform->world_rot, &xform->world_scl);
}

static void
generate_view_matrix(fe_mat4x4_t *dst, const fe_vec3_t *trl, const fe_vec3_t *front, const fe_vec3_t *up)
{
	fe_vec3_t center; 
    fe_vec3_add_to(&center, trl, front);

    *dst = fe_mat4x4_look_at(trl, &center, up);
}

void
fe_transform_generate_view_matrix(const fe_transform_t *xform, fe_mat4x4_t *dst)
{    
    fe_vec3_t front, up;
    fe_transform_get_forward(xform, &front);
    fe_transform_get_up(xform, &up);

    generate_view_matrix(dst, fe_transform_get_translation(xform), &front, &up);
}

void
fe_transform_generate_world_view_matrix(const fe_transform_t *xform, fe_mat4x4_t *dst)
{    
    fe_vec3_t front, up;
    fe_transform_get_world_forward(xform, &front);
    fe_transform_get_world_up(xform, &up);

    generate_view_matrix(dst, fe_transform_get_world_translation(xform), &front, &up);
}


const fe_vec3_t*
fe_transform_get_translation(const fe_transform_t *xform)
{
    return &xform->trl;
}

const fe_quat_t*
fe_transform_get_rotation(const fe_transform_t *xform)
{
    return &xform->rot;
}

const fe_vec3_t*
fe_transform_get_scale(const fe_transform_t *xform)
{
    return &xform->scl;
}

const fe_vec3_t*
fe_transform_get_world_translation(const fe_transform_t *xform)
{
    return &xform->world_trl;
}

const fe_quat_t*
fe_transform_get_world_rotation(const fe_transform_t *xform)
{
    return &xform->world_rot;
}

const fe_vec3_t*
fe_transform_get_world_scale(const fe_transform_t *xform)
{
    return &xform->world_scl;
}

void
fe_transform_set_translation(fe_transform_t *xform, const fe_vec3_t *trl)
{
    xform->trl      = *trl;
    xform->flags   |= FE_TRANSFORM_DIRTY_LOCAL;
}

void
fe_transform_set_rotation(fe_transform_t *xform, const fe_quat_t *rot)
{
    xform->rot      = *rot;
    xform->flags   |= FE_TRANSFORM_DIRTY_LOCAL;
}

void
fe_transform_set_scale(fe_transform_t *xform, const fe_vec3_t *scl)
{
    xform->scl      = *scl;
    xform->flags   |= FE_TRANSFORM_DIRTY_LOCAL;
}

void
fe_transform_set_world_matrix(fe_transform_t *xform, const fe_mat4x4_t *m)
{ 
    xform->world_matrix = *m;
}

const fe_mat4x4_t*
fe_transform_get_world_matrix(const fe_transform_t *xform)
{
    return &xform->world_matrix;
}

void
fe_transform_get_forward(const fe_transform_t *xform, fe_vec3_t *dst)
{
    *dst = fe_quat_mul_vec3(fe_transform_get_rotation(xform), &FE_VEC3_FORWARD);
    fe_vec3_norm(dst);
}

void
fe_transform_get_right(const fe_transform_t *xform, fe_vec3_t *dst)
{
    *dst = fe_quat_mul_vec3(fe_transform_get_rotation(xform), &FE_VEC3_RIGHT);
    fe_vec3_norm(dst);
}

void
fe_transform_get_up(const fe_transform_t *xform, fe_vec3_t *dst)
{
    *dst = fe_quat_mul_vec3(fe_transform_get_rotation(xform), &FE_VEC3_UP);
    fe_vec3_norm(dst);
}

void
fe_transform_get_world_forward(const fe_transform_t *xform, fe_vec3_t *dst)
{
    *dst = fe_quat_mul_vec3(fe_transform_get_world_rotation(xform), &FE_VEC3_FORWARD);
    fe_vec3_norm(dst);
}

void
fe_transform_get_world_right(const fe_transform_t *xform, fe_vec3_t *dst)
{
    *dst = fe_quat_mul_vec3(fe_transform_get_world_rotation(xform), &FE_VEC3_RIGHT);
    fe_vec3_norm(dst);
}

void
fe_transform_get_world_up(const fe_transform_t *xform, fe_vec3_t *dst)
{
    *dst = fe_quat_mul_vec3(fe_transform_get_world_rotation(xform), &FE_VEC3_UP);
    fe_vec3_norm(dst);
}

void    
fe_transform_get_ray(const fe_transform_t *xform, fe_ray_t *dst)
{
    fe_vec3_t forward;
    fe_transform_get_forward(xform, &forward);
    *dst = fe_ray(xform->trl, forward); 
}

void    
fe_transform_get_world_ray(const fe_transform_t *xform, fe_ray_t *dst)
{
    fe_vec3_t forward;
    fe_transform_get_forward(xform, &forward);
    *dst = fe_ray(*fe_transform_get_world_translation(xform), forward); 
}

void
fe_transform_print(const fe_transform_t *xform)
{
    printf("--- Local | Dirty %u ---\n", xform->flags & FE_TRANSFORM_DIRTY_LOCAL);
    fe_vec3_print(&xform->trl);
    fe_quat_print(&xform->rot);
    fe_vec3_print(&xform->scl);

    printf("--- World ---\n");
    fe_vec3_print(&xform->world_trl);
    fe_quat_print(&xform->world_rot);
    fe_vec3_print(&xform->world_scl);
}

