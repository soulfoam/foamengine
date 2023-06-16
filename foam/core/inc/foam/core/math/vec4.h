#pragma once

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <foam/core/math/math_consts.h>
#include <foam/core/ext/sugar.h>

typedef union
{
    fe_vec4a raw;
    struct 
    {
        f32 x, y, z, w;
    }; 
} fe_vec4_t;

#define fe_vec4(x, y, z, w) ((fe_vec4_t){{x, y, z, w}})
#define FE_VEC4_ZERO     fe_vec4(0, 0, 0, 0)
#define FE_VEC4_ONE      fe_vec4(1, 1, 1, 1)

static inline void
fe_vec4a_dup(fe_vec4a r, const fe_vec4a b) 
{
    r[0] = b[0];
    r[1] = b[1];
    r[2] = b[2];
    r[3] = b[3];
}

static inline void
fe_vec4a_add_to(fe_vec4a r, const fe_vec4a a, const fe_vec4a b) 
{ 
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
    r[3] = a[3] + b[3];
} 

static inline void 
fe_vec4a_add(fe_vec4a r, const fe_vec4a b) 
{ 
    fe_vec4a_add_to(r, r, b); 
} 

static inline void 
fe_vec4a_sub_to(fe_vec4a r, const fe_vec4a a, const fe_vec4a b) 
{ 
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
    r[3] = a[3] - b[3];
} 

static inline void 
fe_vec4a_sub(fe_vec4a r, const fe_vec4a b) 
{ 
    fe_vec4a_sub_to(r, r, b); 
} 

static inline void 
fe_vec4a_div_to(fe_vec4a r, const fe_vec4a a, const fe_vec4a b) 
{ 
    r[0] = a[0] / b[0];
    r[1] = a[1] / b[1];
    r[2] = a[2] / b[2];
    r[3] = a[3] / b[3];
} 

static inline void  
fe_vec4a_div(fe_vec4a r, const fe_vec4a a) 
{ 
    fe_vec4a_div_to(r, r, a); 
} 

static inline void 
fe_vec4a_mul_to(fe_vec4a r, const fe_vec4a a, const fe_vec4a b) 
{ 
    r[0] = a[0] * b[0];
    r[1] = a[1] * b[1];
    r[2] = a[2] * b[2];
    r[3] = a[3] * b[3];
} 

static inline void 
fe_vec4a_mul(fe_vec4a r, const fe_vec4a a) 
{ 
    fe_vec4a_mul_to(r, r, a); 
} 

static inline void 
fe_vec4a_scale_to(fe_vec4a r, const fe_vec4a v, f32 s) 
{ 
    r[0] = v[0] * s;
    r[1] = v[1] * s;
    r[2] = v[2] * s;
    r[3] = v[3] * s;
} 

static inline void 
fe_vec4a_scale(fe_vec4a r, f32 s) 
{ 
    fe_vec4a_scale_to(r, r, s); 
} 

static inline f32 
fe_vec4a_dot(const fe_vec4a a, const fe_vec4a b) 
{ 
    f32 p = 0.0;
    p += a[0] * b[0];
    p += a[1] * b[1];
    p += a[2] * b[2];
    p += a[3] * b[3];
    return p; 
} 

static inline void 
fe_vec4a_negate_to(fe_vec4a r, const fe_vec4a v) 
{ 
    r[0] = -v[0];
    r[1] = -v[1];
    r[2] = -v[2];
    r[3] = -v[3];
} 

static inline void 
fe_vec4a_negate(fe_vec4a r) 
{ 
    fe_vec4a_negate_to(r, r); 
} 

static inline f32 
fe_vec4a_len(const fe_vec4a v) 
{ 
    return sqrtf(fe_vec4a_dot(v, v)); 
} 

static inline void 
fe_vec4a_norm_to(fe_vec4a r, const fe_vec4a v) 
{ 
    f32 l = fe_vec4a_len(v); 
    if (l == 0) return; 
    f32 k = 1.0 / l; 
    fe_vec4a_scale_to(r, v, k); 
} 

static inline void 
fe_vec4a_norm(fe_vec4a r) 
{ 
    fe_vec4a_norm_to(r, r);
} 

static inline void 
fe_vec4a_min_to(fe_vec4a r, const fe_vec4a a, const fe_vec4a b) 
{ 
    r[0] = a[0] < b[0] ? a[0] : b[0];
    r[1] = a[1] < b[1] ? a[1] : b[1];
    r[2] = a[2] < b[2] ? a[2] : b[2];
    r[3] = a[3] < b[3] ? a[3] : b[3];
} 

static inline void 
fe_vec4a_max_to(fe_vec4a r, const fe_vec4a a, const fe_vec4a b) 
{ 
    r[0] = a[0] > b[0] ? a[0] : b[0];
    r[1] = a[1] > b[1] ? a[1] : b[1];
    r[2] = a[2] > b[2] ? a[2] : b[2];
    r[3] = a[3] > b[3] ? a[3] : b[3];
} 

static inline void 
fe_vec4a_zero(fe_vec4a r) 
{ 
    r[0] = 0;
    r[1] = 0;
    r[2] = 0;
    r[3] = 0;
} 

static inline void 
fe_vec4a_one(fe_vec4a r) 
{ 
    r[0] = 1;
    r[1] = 1;
    r[2] = 1;
    r[3] = 1;
} 

static inline void 
fe_vec4a_cross_to(fe_vec4a r, const fe_vec4a a, const fe_vec4a b)
{
    r[0] = a[1] * b[2] - a[2] * b[1];
    r[1] = a[2] * b[0] - a[0] * b[2];
    r[2] = a[0] * b[1] - a[1] * b[0];
    r[3] = 1.0f;
}

static inline void 
fe_vec4a_cross(fe_vec4a r, const fe_vec4a v)
{
    fe_vec4a_cross_to(r, r, v);
}

static inline void 
fe_vec4a_reflect_to(fe_vec4a r, const fe_vec4a v, const fe_vec4a n)
{
    f32 p = 2.0f * fe_vec4a_dot(v, n);
    r[0] = v[0] - p * n[0];
    r[1] = v[1] - p * n[1];
    r[2] = v[2] - p * n[2];
    r[3] = v[3] - p * n[3];
}

static inline void 
fe_vec4a_reflect(fe_vec4a r, const fe_vec4a v)
{
    fe_vec4a_reflect_to(r, r, v);
}

static inline void 
fe_vec4a_lerp_to(fe_vec4a r, const fe_vec4a a, const fe_vec4a b, f32 weight)
{
    fe_vec4a_sub_to(r, b, a);
    fe_vec4a_scale_to(r, r, weight);
    fe_vec4a_add_to(r, r, a);
}

static inline void 
fe_vec4a_lerp(fe_vec4a r, const fe_vec4a v, f32 weight)
{
    fe_vec4a_lerp_to(r, r, v, weight);
}

static inline void 
fe_vec4a_trim_to(fe_vec4a r, const fe_vec4a v, f32 len)
{
    fe_vec4a_scale_to(r, v, FE_MIN(fe_vec4a_len(v), len));
}

static inline void 
fe_vec4a_trim(fe_vec4a r, f32 len)
{
    fe_vec4a_trim_to(r, r, len);
}

static inline f64 
fe_vec4a_dist(const fe_vec4a a, const fe_vec4a b)
{
    f32 dx = a[0] - b[0];
    f32 dy = a[1] - b[1];
    f32 dz = a[2] - b[2];
    f32 dw = a[3] - b[3];
    return sqrtf(dx * dx + dy * dy + dz * dz + dw * dw);
}

static inline void 
fe_vec4a_clamp_to(fe_vec4a r, const fe_vec4a v, f32 low, f32 high)
{
    r[0] = FE_MAX(low, FE_MIN(v[0], high));
    r[1] = FE_MAX(low, FE_MIN(v[1], high));
    r[2] = FE_MAX(low, FE_MIN(v[2], high));
    r[3] = FE_MAX(low, FE_MIN(v[3], high));
}

static inline void 
fe_vec4a_clamp(fe_vec4a r, f32 low, f32 high)
{
    fe_vec4a_clamp_to(r, r, low, high);
}

static inline void 
fe_vec4a_print(const fe_vec4a v) 
{ 
    printf("fe_vec4 [%f %f %f %f]\n", v[0], v[1], v[2], v[3]);
} 

static inline bool 
fe_vec4a_eq(const fe_vec4a a, const fe_vec4a b) 
{ 
    if (!fe_math_float_eq(a[0], b[0], FLT_EPSILON)) return false; 
    if (!fe_math_float_eq(a[1], b[1], FLT_EPSILON)) return false; 
    if (!fe_math_float_eq(a[2], b[2], FLT_EPSILON)) return false; 
    if (!fe_math_float_eq(a[3], b[3], FLT_EPSILON)) return false; 
    return true; 
} 

static inline bool 
fe_vec4a_eq_floor(const fe_vec4a a, const fe_vec4a b) 
{ 
    if (floor(a[0]) != floor(b[0])) return false;
    if (floor(a[1]) != floor(b[1])) return false;
    if (floor(a[2]) != floor(b[2])) return false;
    if (floor(a[3]) != floor(b[3])) return false;
    return true; 
} 

static inline void
fe_vec4_add_to(fe_vec4_t *r, const fe_vec4_t *a, const fe_vec4_t *b) 
{ 
    fe_vec4a_add_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec4_add(fe_vec4_t *r, const fe_vec4_t *b) 
{ 
    fe_vec4_add_to(r, r, b); 
} 

static inline void 
fe_vec4_sub_to(fe_vec4_t *r, const fe_vec4_t *a, const fe_vec4_t *b) 
{ 
    fe_vec4a_sub_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec4_sub(fe_vec4_t *r, const fe_vec4_t *b) 
{ 
    fe_vec4_sub_to(r, r, b); 
} 

static inline void 
fe_vec4_div_to(fe_vec4_t *r, const fe_vec4_t *a, const fe_vec4_t *b) 
{ 
    fe_vec4a_div_to(r->raw, a->raw, b->raw);
} 

static inline void  
fe_vec4_div(fe_vec4_t *r, const fe_vec4_t *a) 
{ 
    fe_vec4_div_to(r, r, a); 
} 

static inline void 
fe_vec4_mul_to(fe_vec4_t *r, const fe_vec4_t *a, const fe_vec4_t *b) 
{ 
    fe_vec4a_mul_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec4_mul(fe_vec4_t *r, const fe_vec4_t *a) 
{ 
    fe_vec4_mul_to(r, r, a); 
} 

static inline void 
fe_vec4_scale_to(fe_vec4_t *r, const fe_vec4_t *v, f32 s) 
{ 
    fe_vec4a_scale_to(r->raw, v->raw, s);
} 

static inline void 
fe_vec4_scale(fe_vec4_t *r, f32 s) 
{ 
    fe_vec4_scale_to(r, r, s); 
} 

static inline f32 
fe_vec4_dot(const fe_vec4_t *a, const fe_vec4_t *b) 
{ 
    return fe_vec4a_dot(a->raw, b->raw);
} 

static inline void 
fe_vec4_negate_to(fe_vec4_t *r, const fe_vec4_t *v) 
{ 
    fe_vec4a_negate_to(r->raw, v->raw);
} 

static inline void 
fe_vec4_negate(fe_vec4_t *r) 
{ 
    fe_vec4_negate_to(r, r); 
} 

static inline f32 
fe_vec4_len(const fe_vec4_t *v) 
{ 
    return fe_vec4a_len(v->raw);
} 

static inline void 
fe_vec4_norm_to(fe_vec4_t *r, const fe_vec4_t *v) 
{ 
    fe_vec4a_norm_to(r->raw, v->raw);
} 

static inline void 
fe_vec4_norm(fe_vec4_t *r) 
{ 
    fe_vec4_norm_to(r, r);
} 

static inline void 
fe_vec4_min_to(fe_vec4_t *r, const fe_vec4_t *a, const fe_vec4_t *b) 
{ 
    fe_vec4a_min_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec4_max_to(fe_vec4_t *r, const fe_vec4_t *a, const fe_vec4_t *b) 
{ 
    fe_vec4a_max_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec4_zero(fe_vec4_t *r) 
{ 
    fe_vec4a_zero(r->raw);
} 

static inline void 
fe_vec4_one(fe_vec4_t *r) 
{ 
    fe_vec4a_one(r->raw);
} 

static inline void 
fe_vec4_cross_to(fe_vec4_t *r, const fe_vec4_t *a, const fe_vec4_t *b)
{
    fe_vec4a_cross_to(r->raw, a->raw, b->raw);
}

static inline void 
fe_vec4_cross(fe_vec4_t *r, const fe_vec4_t *v)
{
    fe_vec4_cross_to(r, r, v);
}

static inline void 
fe_vec4_reflect_to(fe_vec4_t *r, const fe_vec4_t *v, const fe_vec4_t *n)
{
    fe_vec4a_reflect_to(r->raw, v->raw, n->raw);
}

static inline void 
fe_vec4_reflect(fe_vec4_t *r, const fe_vec4_t *v)
{
    fe_vec4_reflect_to(r, r, v);
}

static inline void 
fe_vec4_lerp_to(fe_vec4_t *r, const fe_vec4_t *a, const fe_vec4_t *b, f32 weight)
{
    fe_vec4a_lerp_to(r->raw, a->raw, b->raw, weight);
}

static inline void 
fe_vec4_lerp(fe_vec4_t *r, const fe_vec4_t *v, f32 weight)
{
    fe_vec4_lerp_to(r, r, v, weight);
}

static inline void 
fe_vec4_trim_to(fe_vec4_t *r, const fe_vec4_t *v, f32 len)
{
    fe_vec4a_trim_to(r->raw, v->raw, len);
}

static inline void 
fe_vec4_trim(fe_vec4_t *r, f32 len)
{
    fe_vec4_trim_to(r, r, len);
}

static inline f64 
fe_vec4_dist(const fe_vec4_t *a, const fe_vec4_t *b)
{
    return fe_vec4a_dist(a->raw, b->raw);
}

static inline void 
fe_vec4_clamp_to(fe_vec4_t *r, const fe_vec4_t *v, f32 low, f32 high)
{
    fe_vec4a_clamp_to(r->raw, v->raw, low, high);
}

static inline void 
fe_vec4_clamp(fe_vec4_t *r, f32 low, f32 high)
{
    fe_vec4_clamp_to(r, r, low, high);
}

static inline void 
fe_vec4_print(const fe_vec4_t *v) 
{ 
    fe_vec4a_print(v->raw);
} 

static inline bool 
fe_vec4_eq(const fe_vec4_t *a, const fe_vec4_t *b) 
{ 
    return fe_vec4a_eq(a->raw, b->raw);
} 

static inline bool 
fe_vec4_eq_floor(const fe_vec4_t *a, const fe_vec4_t *b) 
{ 
    return fe_vec4a_eq_floor(a->raw, b->raw);
} 

