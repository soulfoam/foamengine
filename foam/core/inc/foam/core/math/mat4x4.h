#pragma once

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/math/math_consts.h>
#include <foam/core/math/vec2.h>
#include <foam/core/math/vec3.h>
#include <foam/core/math/vec4.h>
#include <foam/core/math/quat.h>

typedef union
{
    fe_mat4x4a raw;
    struct 
    {
        f32 m00, m01, m02, m03;
        f32 m10, m11, m12, m13;
        f32 m20, m21, m22, m23;
        f32 m30, m31, m32, m33;
    }; 
} fe_mat4x4_t;

static inline void 
fe_mat4x4_identity(fe_mat4x4_t *r)
{
    for (i32 i = 0; i < 4; ++i)
        for (i32 j = 0; j < 4; ++j)
            r->raw[i][j] = i==j ? 1.f : 0.f;
}

static inline fe_vec4_t 
fe_mat4x4_row(const fe_mat4x4_t *r, i32 i)
{
    fe_vec4_t v;
    for (i32 k = 0; k < 4; ++k) v.raw[k] = r->raw[k][i];
    return v;
}

static inline fe_vec4_t 
fe_mat4x4_col(const fe_mat4x4_t *r, i32 i)
{
    fe_vec4_t v;
    for (i32 k = 0; k < 4; ++k) v.raw[k] = r->raw[i][k];
    return v;
}

static inline void 
fe_mat4x4_transpose_to(fe_mat4x4_t *r, const fe_mat4x4_t *m)
{
    for (i32 j= 0; j < 4; ++j)
        for (i32 i = 0; i < 4; ++i)
            r->raw[i][j] = m->raw[j][i];
}

static inline void 
fe_mat4x4_transpose(fe_mat4x4_t *r)
{
    fe_mat4x4_transpose_to(r, r);
}

static inline void 
fe_mat4x4_add_to(fe_mat4x4_t *r, const fe_mat4x4_t *a, const fe_mat4x4_t *b)
{
    for (i32 i = 0; i < 4; ++i) fe_vec4a_add_to(r->raw[i], a->raw[i], b->raw[i]);
}

static inline void 
fe_mat4x4_add(fe_mat4x4_t *r, const fe_mat4x4_t *m)
{
    fe_mat4x4_add_to(r, r, m);
}

static inline void 
fe_mat4x4_sub_to(fe_mat4x4_t *r, const fe_mat4x4_t *a, const fe_mat4x4_t *b)
{
    for (i32 i = 0; i < 4; ++i) fe_vec4a_sub_to(r->raw[i], a->raw[i], b->raw[i]);
}

static inline void 
fe_mat4x4_sub(fe_mat4x4_t *r, const fe_mat4x4_t *m)
{
    fe_mat4x4_sub_to(r, r, m);
}

static inline void 
fe_mat4x4_scale_to(fe_mat4x4_t *r, const fe_mat4x4_t *a, const fe_vec3_t *s)
{
    fe_vec4a_scale_to(r->raw[0], a->raw[0], s->raw[0]);
    fe_vec4a_scale_to(r->raw[1], a->raw[1], s->raw[1]);
    fe_vec4a_scale_to(r->raw[2], a->raw[2], s->raw[2]);

    for (i32 i = 0; i < 4; ++i) r->raw[3][i] = a->raw[3][i];
}

static inline void 
fe_mat4x4_scale(fe_mat4x4_t *r, const fe_vec3_t *s)
{
    fe_mat4x4_scale_to(r, r, s);
}

static inline void 
fe_mat4x4_scale_n(fe_mat4x4_t *r, const fe_mat4x4_t *m, f32 k)
{
    for (i32 i = 0; i < 4; ++i) fe_vec4a_scale_to(r->raw[i], m->raw[i], k);
}

static inline void 
fe_mat4x4_scale_xyz(fe_mat4x4_t *r, const fe_vec3_t *s)
{
    fe_mat4x4_identity(r);
    r->raw[0][0] = s->raw[0];
    r->raw[1][1] = s->raw[1];
    r->raw[2][2] = s->raw[2];
}

static inline void 
fe_mat4x4_mul_to(fe_mat4x4_t *r, const fe_mat4x4_t *a, const fe_mat4x4_t *b)
{
    fe_mat4x4_t temp;
    for (i32 c = 0; c < 4; ++c) for(i32 r = 0; r < 4; ++r) 
    {
        temp.raw[c][r] = 0.f;
        for (i32 k = 0; k < 4; ++k)
            temp.raw[c][r] += a->raw[k][r] * b->raw[c][k];
    }

    *r = temp;
}

static inline void
fe_mat4x4_mul(fe_mat4x4_t *r, const fe_mat4x4_t *m)
{
    fe_mat4x4_mul_to(r, r, m);
}

static inline fe_vec4_t 
fe_mat4x4_mul_vec4(const fe_mat4x4_t *m, const fe_vec4_t *v)
{
    fe_vec4_t r;
    for (i32 j = 0; j < 4; ++j) 
    {
        r.raw[j] = 0.f;
        for(i32 i = 0; i < 4; ++i)
            r.raw[j] += m->raw[i][j] * v->raw[i];
    }

    return r;
}

static inline void 
fe_mat4x4_translate_xyz(fe_mat4x4_t *r, const fe_vec3_t *t)
{
    r->m30 = t->raw[0];
    r->m31 = t->raw[1];
    r->m32 = t->raw[2];
}

static inline void 
fe_mat4x4_translate(fe_mat4x4_t *r, const fe_vec3_t *v)
{
    fe_vec4_t t = fe_vec4(v->x, v->y, v->z, 0);
    for (i32 i = 0; i < 4; ++i) 
    {
        fe_vec4_t row  = fe_mat4x4_row(r, i);
        r->raw[3][i]  += fe_vec4_dot(&row, &t);
    }
}


static inline void
fe_mat4x4_from_vec3_mul_outer(fe_mat4x4_t *r, const fe_vec3_t *a, const fe_vec3_t *b)
{
    for(i32 i = 0; i < 4; ++i) 
        for(i32 j = 0; j < 4; ++j)
            r->raw[i][j] = i < 3 && j < 3 ? a->raw[i] * b->raw[j] : 0.f;
}

static inline void 
fe_mat4x4_rotate_to(fe_mat4x4_t *r, const fe_mat4x4_t *m, const fe_vec3_t *v, f32 angle)
{
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    
    fe_vec3_t u = *v;
    if(fe_vec3_len(&u) > 1e-4) 
    {
        fe_vec3_norm(&u);
        fe_mat4x4_t mt;
        fe_mat4x4_from_vec3_mul_outer(&mt, &u, &u);

        fe_mat4x4_t ms = 
        {
            {
                {    0,  u.z, -u.y, 0},
                { -u.z,    0,  u.x, 0},
                {  u.y, -u.x,    0, 0},
                {    0,    0,    0, 0} 
            }
        };
        fe_mat4x4_scale_n(&ms, &ms, s);

        fe_mat4x4_t mc;
        fe_mat4x4_identity(&mc);
        fe_mat4x4_sub_to(&mc, &mc, &mt);

        fe_mat4x4_scale_n(&mc, &mc, c);

        fe_mat4x4_add_to(&mt, &mt, &mc);
        fe_mat4x4_add_to(&mt, &mt, &ms);

        mt.raw[3][3] = 1.0;   
        fe_mat4x4_mul_to(r, m, &mt);
    } 
    else 
    {
        *r = *m;
    }
}

static inline void 
fe_mat4x4_rotate(fe_mat4x4_t *r, const fe_vec3_t *v, f32 angle)
{
    fe_mat4x4_rotate_to(r, r, v, angle);
}

static inline void 
fe_mat4x4_rotate_x_to(fe_mat4x4_t *r, const fe_mat4x4_t *m, f32 angle)
{
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    fe_mat4x4_t mat = 
    {
        {
            {1.f, 0.f, 0.f, 0.f},
            {0.f,   c,   s, 0.f},
            {0.f,  -s,   c, 0.f},
            {0.f, 0.f, 0.f, 1.f}
        }
    };
    fe_mat4x4_mul_to(r, m, &mat);
}

static inline void 
fe_mat4x4_rotate_x(fe_mat4x4_t *r, f32 angle)
{
    fe_mat4x4_rotate_x_to(r, r, angle);
}

static inline void 
fe_mat4x4_rotate_y_to(fe_mat4x4_t *r, const fe_mat4x4_t *m, f32 angle)
{
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    fe_mat4x4_t mat = 
    {
        {
            {   c, 0.f,  -s, 0.f},
            { 0.f, 1.f, 0.f, 0.f},
            {   s, 0.f,   c, 0.f},
            { 0.f, 0.f, 0.f, 1.f}
        }
    };

    fe_mat4x4_mul_to(r, m, &mat);
}

static inline void 
fe_mat4x4_rotate_y(fe_mat4x4_t *r, f32 angle)
{
    fe_mat4x4_rotate_y_to(r, r, angle);
}

static inline void 
fe_mat4x4_rotate_z_to(fe_mat4x4_t *r, const fe_mat4x4_t *m, f32 angle)
{
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    fe_mat4x4_t mat = 
    {
        {
            {   c,   s, 0.f, 0.f},
            {  -s,   c, 0.f, 0.f},
            { 0.f, 0.f, 1.f, 0.f},
            { 0.f, 0.f, 0.f, 1.f} 
        }
    };

    fe_mat4x4_mul_to(r, m, &mat);
}

static inline void 
fe_mat4x4_rotate_z(fe_mat4x4_t *r, f32 angle)
{
    fe_mat4x4_rotate_z_to(r, r, angle);
}

static inline void 
fe_mat4x4_invert_to(fe_mat4x4_t *r, const fe_mat4x4_t *m)
{
    f32 s[6];
    f32 c[6];
    s[0] = m->raw[0][0] * m->raw[1][1] - m->raw[1][0] * m->raw[0][1];
    s[1] = m->raw[0][0] * m->raw[1][2] - m->raw[1][0] * m->raw[0][2];
    s[2] = m->raw[0][0] * m->raw[1][3] - m->raw[1][0] * m->raw[0][3];
    s[3] = m->raw[0][1] * m->raw[1][2] - m->raw[1][1] * m->raw[0][2];
    s[4] = m->raw[0][1] * m->raw[1][3] - m->raw[1][1] * m->raw[0][3];
    s[5] = m->raw[0][2] * m->raw[1][3] - m->raw[1][2] * m->raw[0][3];

    c[0] = m->raw[2][0] * m->raw[3][1] - m->raw[3][0] * m->raw[2][1];
    c[1] = m->raw[2][0] * m->raw[3][2] - m->raw[3][0] * m->raw[2][2];
    c[2] = m->raw[2][0] * m->raw[3][3] - m->raw[3][0] * m->raw[2][3];
    c[3] = m->raw[2][1] * m->raw[3][2] - m->raw[3][1] * m->raw[2][2];
    c[4] = m->raw[2][1] * m->raw[3][3] - m->raw[3][1] * m->raw[2][3];
    c[5] = m->raw[2][2] * m->raw[3][3] - m->raw[3][2] * m->raw[2][3];

    /* Assumes it is invertible */
    f32 idet = 1.0f/( s[0]*c[5]-s[1]*c[4]+s[2]*c[3]+s[3]*c[2]-s[4]*c[1]+s[5]*c[0] );

    r->raw[0][0] = ( m->raw[1][1] * c[5] - m->raw[1][2] * c[4] + m->raw[1][3] * c[3]) * idet;
    r->raw[0][1] = (-m->raw[0][1] * c[5] + m->raw[0][2] * c[4] - m->raw[0][3] * c[3]) * idet;
    r->raw[0][2] = ( m->raw[3][1] * s[5] - m->raw[3][2] * s[4] + m->raw[3][3] * s[3]) * idet;
    r->raw[0][3] = (-m->raw[2][1] * s[5] + m->raw[2][2] * s[4] - m->raw[2][3] * s[3]) * idet;

    r->raw[1][0] = (-m->raw[1][0] * c[5] + m->raw[1][2] * c[2] - m->raw[1][3] * c[1]) * idet;
    r->raw[1][1] = ( m->raw[0][0] * c[5] - m->raw[0][2] * c[2] + m->raw[0][3] * c[1]) * idet;
    r->raw[1][2] = (-m->raw[3][0] * s[5] + m->raw[3][2] * s[2] - m->raw[3][3] * s[1]) * idet;
    r->raw[1][3] = ( m->raw[2][0] * s[5] - m->raw[2][2] * s[2] + m->raw[2][3] * s[1]) * idet;

    r->raw[2][0] = ( m->raw[1][0] * c[4] - m->raw[1][1] * c[2] + m->raw[1][3] * c[0]) * idet;
    r->raw[2][1] = (-m->raw[0][0] * c[4] + m->raw[0][1] * c[2] - m->raw[0][3] * c[0]) * idet;
    r->raw[2][2] = ( m->raw[3][0] * s[4] - m->raw[3][1] * s[2] + m->raw[3][3] * s[0]) * idet;
    r->raw[2][3] = (-m->raw[2][0] * s[4] + m->raw[2][1] * s[2] - m->raw[2][3] * s[0]) * idet;

    r->raw[3][0] = (-m->raw[1][0] * c[3] + m->raw[1][1] * c[1] - m->raw[1][2] * c[0]) * idet;
    r->raw[3][1] = ( m->raw[0][0] * c[3] - m->raw[0][1] * c[1] + m->raw[0][2] * c[0]) * idet;
    r->raw[3][2] = (-m->raw[3][0] * s[3] + m->raw[3][1] * s[1] - m->raw[3][2] * s[0]) * idet;
    r->raw[3][3] = ( m->raw[2][0] * s[3] - m->raw[2][1] * s[1] + m->raw[2][2] * s[0]) * idet;
}

static inline void 
fe_mat4x4_invert(fe_mat4x4_t *r)
{
    fe_mat4x4_invert_to(r, r);
}

static inline fe_mat4x4_t 
fe_mat4x4_orthonormalize(const fe_mat4x4_t *m)
{
    fe_mat4x4_t r = *m;
	f32 s = 1.;
	fe_vec3_t h;

    fe_vec3a_norm(r.raw[2]);

	s = fe_vec3a_dot(r.raw[1], r.raw[2]);
	fe_vec3a_scale_to(h.raw, r.raw[2], s);
	fe_vec3a_sub(r.raw[1], h.raw);
	fe_vec3a_norm(r.raw[1]);

	s = fe_vec3a_dot(r.raw[0], r.raw[2]);
	fe_vec3a_scale_to(h.raw, r.raw[2], s);
	fe_vec3a_sub(r.raw[0], h.raw);

	s = fe_vec3a_dot(r.raw[0], r.raw[1]);
	fe_vec3a_scale_to(h.raw, r.raw[1], s);
	fe_vec3a_sub(r.raw[0], h.raw);
	fe_vec3a_norm(r.raw[0]);

    return r;
}

static inline fe_mat4x4_t 
fe_mat4x4_frustum(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
{
    fe_mat4x4_t m;
    fe_mat4x4_identity(&m);

    m.raw[0][0] = 2.f*n/(r-l);
    m.raw[0][1] = m.raw[0][2] = m.raw[0][3] = 0.f;

    m.raw[1][1] = 2.*n/(t-b);
    m.raw[1][0] = m.raw[1][2] = m.raw[1][3] = 0.f;

    m.raw[2][0] = (r+l)/(r-l);
    m.raw[2][1] = (t+b)/(t-b);
    m.raw[2][2] = -(f+n)/(f-n);
    m.raw[2][3] = -1.f;

    m.raw[3][2] = -2.f*(f*n)/(f-n);
    m.raw[3][0] = m.raw[3][1] = m.raw[3][3] = 0.f;

    return m;
}

static inline fe_mat4x4_t 
fe_mat4x4_ortho(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f)
{
    fe_mat4x4_t m;
    fe_mat4x4_identity(&m);

    m.raw[0][0] = 2.f/(r-l);
    m.raw[0][1] = m.raw[0][2] = m.raw[0][3] = 0.f;

    m.raw[1][1] = 2.f/(t-b);
    m.raw[1][0] = m.raw[1][2] = m.raw[1][3] = 0.f;

    m.raw[2][2] = -2.f/(f-n);
    m.raw[2][0] = m.raw[2][1] = m.raw[2][3] = 0.f;

    m.raw[3][0] = -(r+l)/(r-l);
    m.raw[3][1] = -(t+b)/(t-b);
    m.raw[3][2] = -(f+n)/(f-n);
    m.raw[3][3] = 1.f;

    return m;
}

static inline fe_mat4x4_t 
fe_mat4x4_perspective(f32 y_fov, f32 aspect, f32 n, f32 f)
{
    f32 const a = 1.f / tan(y_fov / 2.f);

    fe_mat4x4_t r;
    r.raw[0][0] = a / aspect;
    r.raw[0][1] = 0.f;
    r.raw[0][2] = 0.f;
    r.raw[0][3] = 0.f;

    r.raw[1][0] = 0.f;
    r.raw[1][1] = a;
    r.raw[1][2] = 0.f;
    r.raw[1][3] = 0.f;

    r.raw[2][0] = 0.f;
    r.raw[2][1] = 0.f;
    r.raw[2][2] = -((f + n) / (f - n));
    r.raw[2][3] = -1.f;

    r.raw[3][0] = 0.f;
    r.raw[3][1] = 0.f;
    r.raw[3][2] = -((2.f * f * n) / (f - n));
    r.raw[3][3] = 0.f;

    return r;
}

static inline fe_mat4x4_t 
fe_mat4x4_look_at(const fe_vec3_t *eye, const fe_vec3_t *center, const fe_vec3_t *up)
{
    /* Adapted from Android's OpenGL Matrix.java.                        */
    /* See the OpenGL GLUT documentation for gluLookAt for a description */
    /* of the algorithm. We implement it in a straightforward way:       */

    /* TODO: The negation of of can be spared by swapping the order of
     *       operands in the following cross products in the right way. */
    fe_vec3_t f;
    fe_vec3_sub_to(&f, center, eye); 
    fe_vec3_norm(&f);  

    fe_vec3_t s;
    fe_vec3_cross_to(&s, &f, up);
    fe_vec3_norm(&s);

    fe_vec3_t t;
    fe_vec3_cross_to(&t, &s, &f);

    fe_mat4x4_t r;
    r.raw[0][0] =  s.raw[0];
    r.raw[0][1] =  t.raw[0];
    r.raw[0][2] = -f.raw[0];
    r.raw[0][3] =   0.f;

    r.raw[1][0] =  s.raw[1];
    r.raw[1][1] =  t.raw[1];
    r.raw[1][2] = -f.raw[1];
    r.raw[1][3] =   0.f;

    r.raw[2][0] =  s.raw[2];
    r.raw[2][1] =  t.raw[2];
    r.raw[2][2] = -f.raw[2];
    r.raw[2][3] =   0.f;

    r.raw[3][0] =  0.f;
    r.raw[3][1] =  0.f;
    r.raw[3][2] =  0.f;
    r.raw[3][3] =  1.f;

    fe_vec3_t e = fe_vec3(-eye->x, -eye->y, -eye->z);
    fe_mat4x4_translate(&r, &e);

    return r;
}

static inline fe_mat4x4_t
fe_mat4x4_from_quat(const fe_quat_t *q)
{
    fe_mat4x4_t r;
    fe_mat4x4_identity(&r);
    f32 a = q->raw[3];
    f32 b = q->raw[0];
    f32 c = q->raw[1];
    f32 d = q->raw[2];
    f32 a2 = a*a;
    f32 b2 = b*b;
    f32 c2 = c*c;
    f32 d2 = d*d;

    r.raw[0][0] = a2 + b2 - c2 - d2;
    r.raw[0][1] = 2.f*(b*c + a*d);
    r.raw[0][2] = 2.f*(b*d - a*c);
    r.raw[0][3] = 0.f;

    r.raw[1][0] = 2.f*(b*c - a*d);
    r.raw[1][1] = a2 - b2 + c2 - d2;
    r.raw[1][2] = 2.f*(c*d + a*b);
    r.raw[1][3] = 0.f;

    r.raw[2][0] = 2.f*(b*d + a*c);
    r.raw[2][1] = 2.f*(c*d - a*b);
    r.raw[2][2] = a2 - b2 - c2 + d2;
    r.raw[2][3] = 0.f;

    r.raw[3][0] = r.raw[3][1] = r.raw[3][2] = 0.f;
    r.raw[3][3] = 1.f;

    return r;
}

static inline void 
fe_mat4x4_rotate_quat_to(fe_mat4x4_t *r, const fe_mat4x4_t *a, const fe_quat_t *q)
{
    fe_mat4x4_t rot_mat = fe_mat4x4_from_quat(q);
    fe_mat4x4_mul_to(r, a, &rot_mat);
}

static inline void 
fe_mat4x4_rotate_quat(fe_mat4x4_t *r, const fe_quat_t *q)
{ 
    fe_mat4x4_rotate_quat_to(r, r, q);
}

static inline void 
fe_mat4x4_rotate_quat2(fe_mat4x4_t *r, const fe_quat_t *q) 
{
    f32 qxx = q->raw[0] * q->raw[0];
    f32 qyy = q->raw[1] * q->raw[1];
    f32 qzz = q->raw[2] * q->raw[2];
    f32 qxz = q->raw[0] * q->raw[2];
    f32 qxy = q->raw[0] * q->raw[1];
    f32 qyz = q->raw[1] * q->raw[2];
    f32 qwx = q->raw[3] * q->raw[0];
    f32 qwy = q->raw[3] * q->raw[1];
    f32 qwz = q->raw[3] * q->raw[2];

    r->raw[0][0]  = 1 - 2 * (qyy +  qzz);
    r->raw[1][0]  = 2 * (qxy + qwz);
    r->raw[2][0]  = 2 * (qxz - qwy);
    r->raw[0][1]  = 2 * (qxy - qwz);
    r->raw[1][1]  = 1 - 2 * (qxx +  qzz);
    r->raw[2][1]  = 2 * (qyz + qwx);
    r->raw[0][2]  = 2 * (qxz + qwy);
    r->raw[1][2]  = 2 * (qyz - qwx);
    r->raw[2][2]  = 1 - 2 * (qxx +  qyy);
}

static inline void 
fe_mat4x4o_mul_quat(fe_mat4x4_t *r, const fe_mat4x4_t *m, const fe_quat_t *q)
{
    /*  XXX: The way this is written only works for orthogonal matrices. */
    /* TODO: Take care of non-orthogonal case. */
    fe_quat_mul_vec3a(r->raw[0], q, m->raw[0]);
    fe_quat_mul_vec3a(r->raw[1], q, m->raw[1]);
    fe_quat_mul_vec3a(r->raw[2], q, m->raw[2]);

    r->raw[3][0] = r->raw[3][1] = r->raw[3][2] = 0.f;
    r->raw[3][3] = 1.f;
}


static inline void 
fe_mat4x4_arcball(fe_mat4x4_t *r, const fe_mat4x4_t *m, const fe_vec2_t *_a, const fe_vec2_t *_b, float s)
{
	fe_vec2_t a = *_a;
	fe_vec2_t b = *_b;

	float z_a = 0.;
	float z_b = 0.;

	if(fe_vec2_len(&a) < 1.)  z_a = sqrtf(1. - fe_vec2_dot(&a, &a));
	else                      fe_vec2_norm(&a);

	if(fe_vec2_len(&b) < 1.) z_b = sqrtf(1. - fe_vec2_dot(&b, &b));
	else                     fe_vec2_norm(&b);
	

	fe_vec3_t a_ = fe_vec3(a.x, a.y, z_a);
	fe_vec3_t b_ = fe_vec3(b.x, b.y, z_b);

	fe_vec3_t c_;
	fe_vec3_cross_to(&c_, &a_, &b_);

	float const angle = acos(fe_vec3_dot(&a_, &b_)) * s;
	fe_mat4x4_rotate_to(r, m, &c_, angle);
}

static inline fe_quat_t 
fe_quat_from_mat4x4(const fe_mat4x4_t *mat)
{
    float tr = mat->m00 + mat->m11 + mat->m22;

    fe_quat_t ret;

    if (tr > 0)
    {
        float s = sqrt(tr+1.0) * 2; // s=4*ret.w
        ret.w = 0.25 * s;
        ret.x = (mat->m21 - mat->m12) / s;
        ret.y = (mat->m02 - mat->m20) / s;
        ret.z = (mat->m10 - mat->m01) / s;
    }
    else if ((mat->m00 > mat->m11)&(mat->m00 > mat->m22))
    {
        float s = sqrt(1.0 + mat->m00 - mat->m11 - mat->m22) * 2;
        ret.w = (mat->m21 - mat->m12) / s;
        ret.x = 0.25 * s;
        ret.y = (mat->m01 + mat->m10) / s;
        ret.z = (mat->m02 + mat->m20) / s;
    }
    else if (mat->m11 > mat->m22)
    {
        float s = sqrt(1.0 + mat->m11 - mat->m00 - mat->m22) * 2;
        ret.w = (mat->m02 - mat->m20) / s;
        ret.x = (mat->m01 + mat->m10) / s;
        ret.y = 0.25 * s;
        ret.z = (mat->m12 + mat->m21) / s;
    }
    else
    {
        float s = sqrt(1.0 + mat->m22 - mat->m00 - mat->m11) * 2;
        ret.w = (mat->m10 - mat->m01) / s;
        ret.x = (mat->m02 + mat->m20) / s;
        ret.y = (mat->m12 + mat->m21) / s;
        ret.z = 0.25 * s;
    }

    return ret;

}


static inline void
fe_mat4x4_init(fe_mat4x4_t *dst, const fe_vec3_t *trl, const fe_quat_t *rot, const fe_vec3_t *scl)
{ 
    fe_mat4x4_identity(dst);
    fe_mat4x4_translate(dst, trl);
    fe_mat4x4_rotate_quat(dst, rot);
    fe_mat4x4_scale(dst, scl);
}

static inline fe_mat4x4_t
fe_mat4x4(void)
{ 
    fe_mat4x4_t dst;
    fe_mat4x4_identity(&dst);
    return dst;
}

static inline fe_vec3_t 
fe_mat4x4_get_translation(const fe_mat4x4_t *m)
{
    return fe_vec3(m->m30, m->m31, m->m32);
}


static inline fe_quat_t 
fe_mat4x4_get_rotation(const fe_mat4x4_t *m)
{
    return fe_quat_from_mat4x4(m);
}

static inline fe_vec3_t 
fe_mat4x4_get_scale(const fe_mat4x4_t *m)
{
    f32 scale = sqrtf(m->m00 * m->m00 + m->m01 * m->m01 + m->m02 * m->m02);
    return fe_vec3(scale, scale, scale);
}

static inline void
fe_mat4x4_print(const fe_mat4x4_t *m)
{
    for (u32 i = 0; i < 4; i++)
    {
        for (u32 j = 0; j < 4; j++)
        {
            printf("%f, ", m->raw[j][i]);
        }
        printf("\n");
    }
    printf("\n");
}

