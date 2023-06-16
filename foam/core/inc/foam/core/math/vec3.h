#pragma once

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <foam/core/math/math_consts.h>
#include <foam/core/ext/sugar.h>

typedef union
{
    fe_vec3a raw;
    struct 
    {
        f32 x, y, z;
    }; 
} fe_vec3_t;

#define fe_vec3(x, y, z) ((fe_vec3_t){{x, y, z}})

#define FE_VEC3_ZERO     fe_vec3(0, 0, 0)
#define FE_VEC3_ONE      fe_vec3(1, 1, 1)

#define FE_VEC3_RIGHT   fe_vec3(1, 0, 0)
#define FE_VEC3_FORWARD fe_vec3(0, 1, 0)
#define FE_VEC3_UP      fe_vec3(0, 0, 1)


static inline void
fe_vec3a_add_to(fe_vec3a r, const fe_vec3a a, const fe_vec3a b) 
{ 
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
} 

static inline void 
fe_vec3a_add(fe_vec3a r, const fe_vec3a b) 
{ 
    fe_vec3a_add_to(r, r, b); 
} 

static inline void 
fe_vec3a_sub_to(fe_vec3a r, const fe_vec3a a, const fe_vec3a b) 
{ 
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
} 

static inline void 
fe_vec3a_sub(fe_vec3a r, const fe_vec3a b) 
{ 
    fe_vec3a_sub_to(r, r, b); 
} 

static inline void 
fe_vec3a_div_to(fe_vec3a r, const fe_vec3a a, const fe_vec3a b) 
{ 
    r[0] = a[0] / b[0];
    r[1] = a[1] / b[1];
    r[2] = a[2] / b[2];
} 

static inline void  
fe_vec3a_div(fe_vec3a r, const fe_vec3a a) 
{ 
    fe_vec3a_div_to(r, r, a); 
} 

static inline void 
fe_vec3a_mul_to(fe_vec3a r, const fe_vec3a a, const fe_vec3a b) 
{ 
    r[0] = a[0] * b[0];
    r[1] = a[1] * b[1];
    r[2] = a[2] * b[2];
} 

static inline void 
fe_vec3a_mul(fe_vec3a r, const fe_vec3a a) 
{ 
    fe_vec3a_mul_to(r, r, a); 
} 

static inline void 
fe_vec3a_scale_to(fe_vec3a r, const fe_vec3a v, f32 s) 
{ 
    r[0] = v[0] * s;
    r[1] = v[1] * s;
    r[2] = v[2] * s;
} 

static inline void 
fe_vec3a_scale(fe_vec3a r, f32 s) 
{ 
    fe_vec3a_scale_to(r, r, s); 
} 

static inline f32 
fe_vec3a_dot(const fe_vec3a a, const fe_vec3a b) 
{ 
    f32 p = 0.0;
    p += a[0] * b[0];
    p += a[1] * b[1];
    p += a[2] * b[2];
    return p; 
} 

static inline void 
fe_vec3a_negate_to(fe_vec3a r, const fe_vec3a v) 
{ 
    r[0] = -v[0];
    r[1] = -v[1];
    r[2] = -v[2];
} 

static inline void 
fe_vec3a_negate(fe_vec3a r) 
{ 
    fe_vec3a_negate_to(r, r); 
} 

static inline f32 
fe_vec3a_len(const fe_vec3a v) 
{ 
    return sqrtf(fe_vec3a_dot(v, v)); 
} 

static inline void 
fe_vec3a_norm_to(fe_vec3a r, const fe_vec3a v) 
{ 
    f32 l = fe_vec3a_len(v); 
    if (l == 0) return; 
    f32 k = 1.0 / l; 
    fe_vec3a_scale_to(r, v, k); 
} 

static inline void 
fe_vec3a_norm(fe_vec3a r) 
{ 
    fe_vec3a_norm_to(r, r);
} 

static inline void 
fe_vec3a_min_to(fe_vec3a r, const fe_vec3a a, const fe_vec3a b) 
{ 
    r[0] = a[0] < b[0] ? a[0] : b[0];
    r[1] = a[1] < b[1] ? a[1] : b[1];
    r[2] = a[2] < b[2] ? a[2] : b[2];
} 

static inline void 
fe_vec3a_max_to(fe_vec3a r, const fe_vec3a a, const fe_vec3a b) 
{ 
    r[0] = a[0] > b[0] ? a[0] : b[0];
    r[1] = a[1] > b[1] ? a[1] : b[1];
    r[2] = a[2] > b[2] ? a[2] : b[2];
} 

static inline void 
fe_vec3a_zero(fe_vec3a r) 
{ 
    r[0] = 0;
    r[1] = 0;
    r[2] = 0;
} 

static inline void 
fe_vec3a_one(fe_vec3a r) 
{ 
    r[0] = 1;
    r[1] = 1;
    r[2] = 1;
} 

static inline void 
fe_vec3a_cross_to(fe_vec3a r, const fe_vec3a a, const fe_vec3a b)
{
    r[0] = a[1] * b[2] - a[2] * b[1];
    r[1] = a[2] * b[0] - a[0] * b[2];
    r[2] = a[0] * b[1] - a[1] * b[0];
}

static inline void 
fe_vec3a_cross(fe_vec3a r, const fe_vec3a v)
{
    fe_vec3a_cross_to(r, r, v);
}

static inline void 
fe_vec3a_reflect_to(fe_vec3a r, const fe_vec3a v, const fe_vec3a n)
{
    f32 p = 2.0f * fe_vec3a_dot(v, n);
    r[0] = v[0] - p * n[0];
    r[1] = v[1] - p * n[1];
    r[2] = v[2] - p * n[2];
}

static inline void 
fe_vec3a_reflect(fe_vec3a r, const fe_vec3a v)
{
    fe_vec3a_reflect_to(r, r, v);
}

static inline void 
fe_vec3a_lerp_to(fe_vec3a r, const fe_vec3a a, const fe_vec3a b, f32 weight)
{
    fe_vec3a_sub_to(r, b, a);
    fe_vec3a_scale_to(r, r, weight);
    fe_vec3a_add_to(r, r, a);
}

static inline void 
fe_vec3a_lerp(fe_vec3a r, const fe_vec3a v, f32 weight)
{
    fe_vec3a_lerp_to(r, r, v, weight);
}

static inline void 
fe_vec3a_trim_to(fe_vec3a r, const fe_vec3a v, f32 len)
{
    fe_vec3a_scale_to(r, v, FE_MIN(fe_vec3a_len(v), len));
}

static inline void 
fe_vec3a_trim(fe_vec3a r, f32 len)
{
    fe_vec3a_trim_to(r, r, len);
}

static inline f64 
fe_vec3a_dist(const fe_vec3a a, const fe_vec3a b)
{
    f32 dx = a[0] - b[0];
    f32 dy = a[1] - b[1];
    f32 dz = a[2] - b[2];
    return sqrtf(dx * dx + dy * dy + dz * dz);
}

static inline void 
fe_vec3a_clamp_to(fe_vec3a r, const fe_vec3a v, f32 low, f32 high)
{
    r[0] = FE_MAX(low, FE_MIN(v[0], high));
    r[1] = FE_MAX(low, FE_MIN(v[1], high));
    r[2] = FE_MAX(low, FE_MIN(v[2], high));
}

static inline void 
fe_vec3a_clamp(fe_vec3a r, f32 low, f32 high)
{
    fe_vec3a_clamp_to(r, r, low, high);
}

static inline void 
fe_vec3a_print(const fe_vec3a v) 
{ 
    printf("fe_vec3 [%f %f %f]\n", v[0], v[1], v[2]);
} 

static inline bool 
fe_vec3a_eq(const fe_vec3a a, const fe_vec3a b) 
{ 
    if (!fe_math_float_eq(a[0], b[0], FLT_EPSILON)) return false; 
    if (!fe_math_float_eq(a[1], b[1], FLT_EPSILON)) return false; 
    if (!fe_math_float_eq(a[2], b[2], FLT_EPSILON)) return false; 
    return true; 
} 

static inline bool 
fe_vec3a_eq_floor(const fe_vec3a a, const fe_vec3a b) 
{ 
    if (floor(a[0]) != floor(b[0])) return false;
    if (floor(a[1]) != floor(b[1])) return false;
    if (floor(a[2]) != floor(b[2])) return false;
    return true; 
} 

static inline void
fe_vec3_add_to(fe_vec3_t *r, const fe_vec3_t *a, const fe_vec3_t *b) 
{ 
    fe_vec3a_add_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec3_add(fe_vec3_t *r, const fe_vec3_t *b) 
{ 
    fe_vec3_add_to(r, r, b); 
} 

static inline void 
fe_vec3_sub_to(fe_vec3_t *r, const fe_vec3_t *a, const fe_vec3_t *b) 
{ 
    fe_vec3a_sub_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec3_sub(fe_vec3_t *r, const fe_vec3_t *b) 
{ 
    fe_vec3_sub_to(r, r, b); 
} 

static inline void 
fe_vec3_div_to(fe_vec3_t *r, const fe_vec3_t *a, const fe_vec3_t *b) 
{ 
    fe_vec3a_div_to(r->raw, a->raw, b->raw);
} 

static inline void  
fe_vec3_div(fe_vec3_t *r, const fe_vec3_t *a) 
{ 
    fe_vec3_div_to(r, r, a); 
} 

static inline void 
fe_vec3_mul_to(fe_vec3_t *r, const fe_vec3_t *a, const fe_vec3_t *b) 
{ 
    fe_vec3a_mul_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec3_mul(fe_vec3_t *r, const fe_vec3_t *a) 
{ 
    fe_vec3_mul_to(r, r, a); 
} 

static inline void 
fe_vec3_scale_to(fe_vec3_t *r, const fe_vec3_t *v, f32 s) 
{ 
    fe_vec3a_scale_to(r->raw, v->raw, s);
} 

static inline void 
fe_vec3_scale(fe_vec3_t *r, f32 s) 
{ 
    fe_vec3_scale_to(r, r, s); 
} 

static inline f32 
fe_vec3_dot(const fe_vec3_t *a, const fe_vec3_t *b) 
{ 
    return fe_vec3a_dot(a->raw, b->raw);
} 

static inline void 
fe_vec3_negate_to(fe_vec3_t *r, const fe_vec3_t *v) 
{ 
    fe_vec3a_negate_to(r->raw, v->raw);
} 

static inline void 
fe_vec3_negate(fe_vec3_t *r) 
{ 
    fe_vec3_negate_to(r, r); 
} 

static inline f32 
fe_vec3_len(const fe_vec3_t *v) 
{ 
    return fe_vec3a_len(v->raw);
} 

static inline void 
fe_vec3_norm_to(fe_vec3_t *r, const fe_vec3_t *v) 
{ 
    return fe_vec3a_norm_to(r->raw, v->raw);
} 

static inline void 
fe_vec3_norm(fe_vec3_t *r) 
{ 
    fe_vec3_norm_to(r, r);
} 

static inline void 
fe_vec3_min_to(fe_vec3_t *r, const fe_vec3_t *a, const fe_vec3_t *b) 
{ 
    fe_vec3a_min_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec3_max_to(fe_vec3_t *r, const fe_vec3_t *a, const fe_vec3_t *b) 
{ 
    fe_vec3a_max_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec3_zero(fe_vec3_t *r) 
{ 
    fe_vec3a_zero(r->raw);
} 

static inline void 
fe_vec3_one(fe_vec3_t *r) 
{ 
    fe_vec3a_one(r->raw);
} 

static inline void 
fe_vec3_cross_to(fe_vec3_t *r, const fe_vec3_t *a, const fe_vec3_t *b)
{
    fe_vec3a_cross_to(r->raw, a->raw, b->raw);
}

static inline void 
fe_vec3_cross(fe_vec3_t *r, const fe_vec3_t *v)
{
    fe_vec3_cross_to(r, r, v);
}

static inline void 
fe_vec3_reflect_to(fe_vec3_t *r, const fe_vec3_t *v, const fe_vec3_t *n)
{
    fe_vec3a_reflect_to(r->raw, v->raw, n->raw);
}

static inline void 
fe_vec3_reflect(fe_vec3_t *r, const fe_vec3_t *v)
{
    fe_vec3_reflect_to(r, r, v);
}

static inline void 
fe_vec3_lerp_to(fe_vec3_t *r, const fe_vec3_t *a, const fe_vec3_t *b, f32 weight)
{
    fe_vec3a_lerp_to(r->raw, a->raw, b->raw, weight);
}

static inline void 
fe_vec3_lerp(fe_vec3_t *r, const fe_vec3_t *v, f32 weight)
{
    fe_vec3_lerp_to(r, r, v, weight);
}

static inline void 
fe_vec3_trim_to(fe_vec3_t *r, const fe_vec3_t *v, f32 len)
{
    fe_vec3a_trim_to(r->raw, v->raw, len);
}

static inline void 
fe_vec3_trim(fe_vec3_t *r, f32 len)
{
    fe_vec3_trim_to(r, r, len);
}

static inline f64 
fe_vec3_dist(const fe_vec3_t *a, const fe_vec3_t *b)
{
    return fe_vec3a_dist(a->raw, b->raw);
}

static inline void 
fe_vec3_clamp_to(fe_vec3_t *r, const fe_vec3_t *v, f32 low, f32 high)
{
    fe_vec3a_clamp_to(r->raw, v->raw, low, high);
}

static inline void 
fe_vec3_clamp(fe_vec3_t *r, f32 low, f32 high)
{
    fe_vec3_clamp_to(r, r, low, high);
}

static inline void 
fe_vec3_print(const fe_vec3_t *v) 
{ 
    fe_vec3a_print(v->raw);
} 

static inline bool 
fe_vec3_eq(const fe_vec3_t *a, const fe_vec3_t *b) 
{ 
    return fe_vec3a_eq(a->raw, b->raw);
} 

static inline bool 
fe_vec3_eq_floor(const fe_vec3_t *a, const fe_vec3_t *b) 
{ 
    return fe_vec3a_eq_floor(a->raw, b->raw);
} 
