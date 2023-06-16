
#pragma once

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <foam/core/math/math_consts.h>
#include <foam/core/ext/sugar.h>

typedef union
{
    fe_vec2a raw;
    struct 
    {
        f32 x, y;
    }; 
} fe_vec2_t;

#define fe_vec2(x, y) ((fe_vec2_t){{x, y}})

#define FE_VEC2_ZERO     fe_vec2(0, 0)
#define FE_VEC2_ONE      fe_vec2(1, 1)

static inline void
fe_vec2a_add_to(fe_vec2a r, const fe_vec2a a, const fe_vec2a b) 
{ 
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
} 

static inline void 
fe_vec2a_add(fe_vec2a r, const fe_vec2a b) 
{ 
    fe_vec2a_add_to(r, r, b); 
} 

static inline void 
fe_vec2a_sub_to(fe_vec2a r, const fe_vec2a a, const fe_vec2a b) 
{ 
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
} 

static inline void 
fe_vec2a_sub(fe_vec2a r, const fe_vec2a b) 
{ 
    fe_vec2a_sub_to(r, r, b); 
} 

static inline void 
fe_vec2a_div_to(fe_vec2a r, const fe_vec2a a, const fe_vec2a b) 
{ 
    r[0] = a[0] / b[0];
    r[1] = a[1] / b[1];
} 

static inline void  
fe_vec2a_div(fe_vec2a r, const fe_vec2a a) 
{ 
    fe_vec2a_div_to(r, r, a); 
} 

static inline void 
fe_vec2a_mul_to(fe_vec2a r, const fe_vec2a a, const fe_vec2a b) 
{ 
    r[0] = a[0] * b[0];
    r[1] = a[1] * b[1];
} 

static inline void 
fe_vec2a_mul(fe_vec2a r, const fe_vec2a a) 
{ 
    fe_vec2a_mul_to(r, r, a); 
} 

static inline void 
fe_vec2a_scale_to(fe_vec2a r, const fe_vec2a v, f32 s) 
{ 
    r[0] = v[0] * s;
    r[1] = v[1] * s;
} 

static inline void 
fe_vec2a_scale(fe_vec2a r, f32 s) 
{ 
    fe_vec2a_scale_to(r, r, s); 
} 

static inline f32 
fe_vec2a_dot(const fe_vec2a a, const fe_vec2a b) 
{ 
    f32 p = 0.0;
    p += a[0] * b[0];
    p += a[1] * b[1];
    return p; 
} 

static inline void 
fe_vec2a_negate_to(fe_vec2a r, const fe_vec2a v) 
{ 
    r[0] = -v[0];
    r[1] = -v[1];
} 

static inline void 
fe_vec2a_negate(fe_vec2a r) 
{ 
    fe_vec2a_negate_to(r, r); 
} 

static inline f32 
fe_vec2a_len(const fe_vec2a v) 
{ 
    return sqrtf(fe_vec2a_dot(v, v)); 
} 

static inline void 
fe_vec2a_norm_to(fe_vec2a r, const fe_vec2a v) 
{ 
    f32 l = fe_vec2a_len(v); 
    if (l == 0) return; 
    f32 k = 1.0 / l; 
    fe_vec2a_scale_to(r, v, k); 
} 

static inline void 
fe_vec2a_norm(fe_vec2a r) 
{ 
    fe_vec2a_norm_to(r, r);
} 

static inline void 
fe_vec2a_min_to(fe_vec2a r, const fe_vec2a a, const fe_vec2a b) 
{ 
    r[0] = a[0] < b[0] ? a[0] : b[0];
    r[1] = a[1] < b[1] ? a[1] : b[1];
} 

static inline void 
fe_vec2a_max_to(fe_vec2a r, const fe_vec2a a, const fe_vec2a b) 
{ 
    r[0] = a[0] > b[0] ? a[0] : b[0];
    r[1] = a[1] > b[1] ? a[1] : b[1];
} 

static inline void 
fe_vec2a_zero(fe_vec2a r) 
{ 
    r[0] = 0;
    r[1] = 0;
} 

static inline void 
fe_vec2a_one(fe_vec2a r) 
{ 
    r[0] = 1;
    r[1] = 1;
} 

static inline f64 
fe_vec2a_cross(const fe_vec2a a, const fe_vec2a b)
{
    return a[0] * b[1] - a[1] * b[0];
}

static inline void 
fe_vec2a_reflect_to(fe_vec2a r, const fe_vec2a v, const fe_vec2a n)
{
    f32 p = 2.0f * fe_vec2a_dot(v, n);
    r[0] = v[0] - p * n[0];
    r[1] = v[1] - p * n[1];
}

static inline void 
fe_vec2a_reflect(fe_vec2a r, const fe_vec2a v)
{
    fe_vec2a_reflect_to(r, r, v);
}

static inline void 
fe_vec2a_lerp_to(fe_vec2a r, const fe_vec2a a, const fe_vec2a b, f32 weight)
{
    fe_vec2a_sub_to(r, b, a);
    fe_vec2a_scale_to(r, r, weight);
    fe_vec2a_add_to(r, r, a);
}

static inline void 
fe_vec2a_lerp(fe_vec2a r, const fe_vec2a v, f32 weight)
{
    fe_vec2a_lerp_to(r, r, v, weight);
}

static inline void 
fe_vec2a_trim_to(fe_vec2a r, const fe_vec2a v, f32 len)
{
    fe_vec2a_scale_to(r, v, FE_MIN(fe_vec2a_len(v), len));
}

static inline void 
fe_vec2a_trim(fe_vec2a r, f32 len)
{
    fe_vec2a_trim_to(r, r, len);
}

static inline f64 
fe_vec2a_dist(const fe_vec2a a, const fe_vec2a b)
{
    f32 dx = a[0] - b[0];
    f32 dy = a[1] - b[1];
    return sqrtf(dx * dx + dy * dy);
}

static inline void 
fe_vec2a_clamp_to(fe_vec2a r, const fe_vec2a v, f32 low, f32 high)
{
    r[0] = FE_MAX(low, FE_MIN(v[0], high));
    r[1] = FE_MAX(low, FE_MIN(v[1], high));
}

static inline void 
fe_vec2a_clamp(fe_vec2a r, f32 low, f32 high)
{
    fe_vec2a_clamp_to(r, r, low, high);
}

static inline void 
fe_vec2a_print(const fe_vec2a v) 
{ 
    printf("fe_vec2 [%f %f]\n", v[0], v[1]);
} 

static inline bool 
fe_vec2a_eq(const fe_vec2a a, const fe_vec2a b) 
{ 
    if (!fe_math_float_eq(a[0], b[0], FLT_EPSILON)) return false; 
    if (!fe_math_float_eq(a[1], b[1], FLT_EPSILON)) return false; 
    return true; 
} 

static inline bool 
fe_vec2a_eq_floor(const fe_vec2a a, const fe_vec2a b) 
{ 
    if (floor(a[0]) != floor(b[0])) return false;
    if (floor(a[1]) != floor(b[1])) return false;
    return true; 
} 


static inline void
fe_vec2_add_to(fe_vec2_t *r, const fe_vec2_t *a, const fe_vec2_t *b) 
{ 
    fe_vec2a_add_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec2_add(fe_vec2_t *r, const fe_vec2_t *b) 
{ 
    fe_vec2_add_to(r, r, b); 
} 

static inline void 
fe_vec2_sub_to(fe_vec2_t *r, const fe_vec2_t *a, const fe_vec2_t *b) 
{ 
    fe_vec2a_sub_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec2_sub(fe_vec2_t *r, const fe_vec2_t *b) 
{ 
    fe_vec2_sub_to(r, r, b); 
} 

static inline void 
fe_vec2_div_to(fe_vec2_t *r, const fe_vec2_t *a, const fe_vec2_t *b) 
{ 
    fe_vec2a_div_to(r->raw, a->raw, b->raw);
} 

static inline void  
fe_vec2_div(fe_vec2_t *r, const fe_vec2_t *a) 
{ 
    fe_vec2_div_to(r, r, a); 
} 

static inline void 
fe_vec2_mul_to(fe_vec2_t *r, const fe_vec2_t *a, const fe_vec2_t *b) 
{ 
    fe_vec2a_mul_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec2_mul(fe_vec2_t *r, const fe_vec2_t *a) 
{ 
    fe_vec2_mul_to(r, r, a); 
} 

static inline void 
fe_vec2_scale_to(fe_vec2_t *r, const fe_vec2_t *v, f32 s) 
{ 
    fe_vec2a_scale_to(r->raw, v->raw, s);
} 

static inline void 
fe_vec2_scale(fe_vec2_t *r, f32 s) 
{ 
    fe_vec2_scale_to(r, r, s); 
} 

static inline f32 
fe_vec2_dot(const fe_vec2_t *a, const fe_vec2_t *b) 
{ 
    return fe_vec2a_dot(a->raw, b->raw);
} 

static inline void 
fe_vec2_negate_to(fe_vec2_t *r, const fe_vec2_t *v) 
{ 
    fe_vec2a_negate_to(r->raw, v->raw);
} 

static inline void 
fe_vec2_negate(fe_vec2_t *r) 
{ 
    fe_vec2_negate_to(r, r); 
} 

static inline f32 
fe_vec2_len(const fe_vec2_t *v) 
{ 
    return fe_vec2a_len(v->raw);
} 

static inline void 
fe_vec2_norm_to(fe_vec2_t *r, const fe_vec2_t *v) 
{ 
    fe_vec2a_norm_to(r->raw, v->raw);
} 

static inline void 
fe_vec2_norm(fe_vec2_t *r) 
{ 
    fe_vec2_norm_to(r, r);
} 

static inline void 
fe_vec2_min_to(fe_vec2_t *r, const fe_vec2_t *a, const fe_vec2_t *b) 
{ 
    fe_vec2a_min_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec2_max_to(fe_vec2_t *r, const fe_vec2_t *a, const fe_vec2_t *b) 
{ 
    fe_vec2a_max_to(r->raw, a->raw, b->raw);
} 

static inline void 
fe_vec2_zero(fe_vec2_t *r) 
{ 
    fe_vec2a_zero(r->raw);
} 

static inline void 
fe_vec2_one(fe_vec2_t *r) 
{ 
    fe_vec2a_one(r->raw);
} 

static inline f64 
fe_vec2_cross(const fe_vec2_t *a, const fe_vec2_t *b)
{
    return fe_vec2a_cross(a->raw, b->raw);
}

static inline void 
fe_vec2_reflect_to(fe_vec2_t *r, const fe_vec2_t *v, const fe_vec2_t *n)
{
    fe_vec2a_reflect_to(r->raw, v->raw, n->raw);
}

static inline void 
fe_vec2_reflect(fe_vec2_t *r, const fe_vec2_t *v)
{
    fe_vec2_reflect_to(r, r, v);
}

static inline void 
fe_vec2_lerp_to(fe_vec2_t *r, const fe_vec2_t *a, const fe_vec2_t *b, f32 weight)
{
    fe_vec2a_lerp_to(r->raw, a->raw, b->raw, weight);
}

static inline void 
fe_vec2_lerp(fe_vec2_t *r, const fe_vec2_t *v, f32 weight)
{
    fe_vec2_lerp_to(r, r, v, weight);
}

static inline void 
fe_vec2_trim_to(fe_vec2_t *r, const fe_vec2_t *v, f32 len)
{
    fe_vec2a_trim_to(r->raw, v->raw, len);
}

static inline void 
fe_vec2_trim(fe_vec2_t *r, f32 len)
{
    fe_vec2_trim_to(r, r, len);
}

static inline f64 
fe_vec2_dist(const fe_vec2_t *a, const fe_vec2_t *b)
{
    return fe_vec2a_dist(a->raw, b->raw);
}

static inline void 
fe_vec2_clamp_to(fe_vec2_t *r, const fe_vec2_t *v, f32 low, f32 high)
{
    fe_vec2a_clamp_to(r->raw, v->raw, low, high);
}

static inline void 
fe_vec2_clamp(fe_vec2_t *r, f32 low, f32 high)
{
    fe_vec2_clamp_to(r, r, low, high);
}

static inline void 
fe_vec2_print(const fe_vec2_t *v) 
{ 
    fe_vec2a_print(v->raw);
} 

static inline bool 
fe_vec2_eq(const fe_vec2_t *a, const fe_vec2_t *b) 
{ 
    return fe_vec2a_eq(a->raw, b->raw);
} 

static inline bool 
fe_vec2_eq_floor(const fe_vec2_t *a, const fe_vec2_t *b) 
{ 
    return fe_vec2a_eq_floor(a->raw, b->raw);
} 

