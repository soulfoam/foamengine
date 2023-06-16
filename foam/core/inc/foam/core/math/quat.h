#pragma once

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <foam/core/math/math_consts.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/math/vec3.h>
#include <foam/core/math/vec4.h>

typedef union
{
    fe_vec4a raw;
    struct 
    {
        f32 x, y, z, w;
    }; 
} fe_quat_t;

#define fe_quat(x, y, z, w) ((fe_quat_t){{x, y, z, w}})
#define FE_QUAT_IDEN fe_quat(0, 0, 0, 1)

static inline void 
fe_quat_print(const fe_quat_t *v) 
{ 
    printf("fe_quat [%f %f %f %f]\n", v->x, v->y, v->z, v->w);
} 

static inline void 
fe_quat_identity(fe_quat_t *q)
{
    q->x = q->y = q->z = 0.0f;
    q->w = 1.0f;
}

static inline void 
fe_quat_add_to(fe_quat_t *r, const fe_quat_t *a, const fe_quat_t *b)
{
    for(i32 i = 0; i < 4; ++i) r->raw[i] = a->raw[i] + b->raw[i];
}

static inline void 
fe_quat_add(fe_quat_t *r, const fe_quat_t *q)
{
    fe_quat_add_to(r, r, q);
}

static inline void 
fe_quat_sub_to(fe_quat_t *r, const fe_quat_t *a, const fe_quat_t *b)
{
    for(i32 i = 0; i < 4; ++i) r->raw[i] = a->raw[i] - b->raw[i];
}

static inline void 
fe_quat_sub(fe_quat_t *r, const fe_quat_t *q)
{
    fe_quat_sub_to(r, r, q);
}

static inline void 
fe_quat_mul_to(fe_quat_t *r, const fe_quat_t *p, const fe_quat_t *q)
{
    r->raw[0] = p->raw[3] * q->raw[0] + p->raw[0] * q->raw[3] + p->raw[1] * q->raw[2] - p->raw[2] * q->raw[1];
    r->raw[1] = p->raw[3] * q->raw[1] - p->raw[0] * q->raw[2] + p->raw[1] * q->raw[3] + p->raw[2] * q->raw[0];
    r->raw[2] = p->raw[3] * q->raw[2] + p->raw[0] * q->raw[1] - p->raw[1] * q->raw[0] + p->raw[2] * q->raw[3];
    r->raw[3] = p->raw[3] * q->raw[3] - p->raw[0] * q->raw[0] - p->raw[1] * q->raw[1] - p->raw[2] * q->raw[2];
}

static inline void 
fe_quat_mul(fe_quat_t *r, const fe_quat_t *q)
{
    fe_quat_mul_to(r, r, q);
}

static inline void 
fe_quat_scale_to(fe_quat_t *r, const fe_quat_t *v, f32 s)
{
    for(i32 i = 0; i < 4; ++i) r->raw[i] = v->raw[i] * s;
}

static inline void 
fe_quat_scale(fe_quat_t *r, f32 s)
{
    fe_quat_scale_to(r, r, s);
}

static inline f32 
fe_quat_inner_product(fe_quat_t *r, const fe_quat_t *b)
{
    f32 p = 0.f;
    for(i32 i = 0; i < 4; ++i) p += b->raw[i] * r->raw[i];
    return p;
}

static inline void 
fe_quat_conj_to(fe_quat_t *r, const fe_quat_t *q)
{
    for(i32 i=0; i < 3; ++i) r->raw[i] = -q->raw[i];
    r[3] = q[3];
}

static inline void 
fe_quat_conj(fe_quat_t *r)
{
    fe_quat_conj_to(r, r);
}

static inline fe_quat_t 
fe_quat_rotation(const fe_vec3_t *axis, f32 angle) 
{
    fe_quat_t r;

    fe_vec3_t v;
    fe_vec3_scale_to(&v, axis, sinf(angle / 2));
    for(i32 i = 0; i < 3; ++i) r.raw[i] = v.raw[i];
    r.raw[3] = cosf(angle / 2);

    return r;
}

static inline fe_quat_t
fe_quat_from_euler(const fe_vec3_t *rot)
{   
    fe_quat_t z = fe_quat_rotation(&fe_vec3(0, 0, 1), fe_math_rad(rot->z));
    fe_quat_t x = fe_quat_rotation(&fe_vec3(1, 0, 0), fe_math_rad(rot->x));
    fe_quat_t y = fe_quat_rotation(&fe_vec3(0, 1, 0), fe_math_rad(rot->y));

    fe_quat_t q;
    fe_quat_mul_to(&q, &z, &x);

    fe_quat_t r;
    fe_quat_mul_to(&r, &q, &y);

    return r;
}

static inline void 
fe_quat_mul_vec3a(fe_vec3a r, const fe_quat_t *q, const fe_vec3a v)
{
    /*
     * Method by Fabian 'ryg' Giessen (of Farbrausch)
     t = 2 * cross(q.xyz, v)
     v' = v + q.w * t + cross(q.xyz, t)
     */
    fe_vec3_t q_xyz = fe_vec3(q->x, q->y, q->z);
    fe_vec3_t u = fe_vec3(q->x, q->y, q->z);

    fe_vec3_t t;
    fe_vec3a_cross_to(t.raw, q_xyz.raw, v);
    fe_vec3_scale(&t, 2);

    fe_vec3_cross_to(&u, &q_xyz, &t);
    fe_vec3_scale(&t, q->w);

    fe_vec3a_add_to(r, v, t.raw);
    fe_vec3a_add(r, u.raw);
}

static inline fe_vec3_t 
fe_quat_mul_vec3(const fe_quat_t *q, const fe_vec3_t *v)
{
    /*
     * Method by Fabian 'ryg' Giessen (of Farbrausch)
     t = 2 * cross(q.xyz, v)
     v' = v + q.w * t + cross(q.xyz, t)
     */
    fe_vec3_t q_xyz = fe_vec3(q->x, q->y, q->z);
    fe_vec3_t u = fe_vec3(q->x, q->y, q->z);

    fe_vec3_t t;
    fe_vec3_cross_to(&t, &q_xyz, v);
    fe_vec3_scale(&t, 2);

    fe_vec3_cross_to(&u, &q_xyz, &t);
    fe_vec3_scale(&t, q->w);

    fe_vec3_t r;
    fe_vec3_add_to(&r, v, &t);
    fe_vec3_add(&r, &u);

    return r;
}

static inline void 
fe_quat_lerp_to(fe_quat_t *r, const fe_quat_t *a, const fe_quat_t *b, f32 weight)
{
    fe_quat_sub_to(r, b, a);
    fe_quat_scale(r, weight);
    fe_quat_add(r, a);
    fe_vec4a_norm(r->raw);
}

static inline void 
fe_quat_lerp(fe_quat_t *r, const fe_quat_t *q, f32 weight)
{
    fe_quat_lerp_to(r, r, q, weight);
}

static inline void 
fe_quat_slerp_to(fe_quat_t *q, const fe_quat_t *from, const fe_quat_t *to, f32 t)
{
    fe_vec4_t q1, q2;
    float cos_theta, sin_theta, angle;

    cos_theta = fe_vec4a_dot(from->raw, to->raw);
    fe_vec4a_dup(q1.raw, from->raw);

    if (fabsf(cos_theta) >= 1.0f) 
    {
        fe_vec4a_dup(q->raw, q1.raw);
        return;
    }

    if (cos_theta < 0.0f) 
    {
        fe_vec4a_negate(q1.raw);
        cos_theta = -cos_theta;
    }

    sin_theta = sqrtf(1.0f - cos_theta * cos_theta);

    /* LERP to avoid zero division */
    if (fabsf(sin_theta) < 0.001f) 
    {
        fe_quat_lerp_to(q, from, to, t);
        return;
    }

    /* SLERP */
    angle = acosf(cos_theta);
    fe_vec4_scale(&q1, sinf((1.0f - t) * angle));
    fe_vec4a_scale_to(q2.raw, to->raw, sinf(t * angle));

    fe_vec4_add(&q1, &q2);
    fe_vec4a_scale_to(q->raw, q1.raw, 1.0f / sin_theta);
}

