#pragma once

#include <math.h>
#include <float.h>
#include <foam/core/ext/sugar.h>

#define FE_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define FE_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define FE_DOT_THRESHOLD 0.9995

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define DOT_THRESHOLD 0.9995

typedef f32      fe_vec2a[2];
typedef f32      fe_vec3a[3];
typedef f32      fe_vec4a[4];
typedef fe_vec4a fe_mat4x4a[4];

static inline bool 
fe_math_float_eq(f32 a, f32 b, f32 epsilon)
{
    f32 absA = fabs(a);
    f32 absB = fabs(b);
    f32 diff = fabs(a - b);

    if (a == b) 
    { 
        return true;
    } 
    else if (a == 0 || b == 0 || (absA + absB < FLT_MIN)) 
    {
        // a or b is zero or both are extremely close to it
        // relative error is less meaningful here
        return diff < (epsilon * FLT_MIN);
    } 
    else 
    { // use relative error
        return diff / FE_MIN((absA + absB), FLT_MIN) < epsilon;
    }
}

static inline f64 
fe_math_median(f64 a, f64 b, f64 c)
{
    return FE_MAX(FE_MIN(a, b), FE_MIN(FE_MAX(a, b), c));
}

static inline i32 
fe_math_non_zero_sign(f64 n)
{
    return 2*(n > 0)-1;
}

static inline f32 
fe_math_lerp(f32 a, f32 b, f32 f)
{
    return a + f * (b - a);
}

static inline f32 
fe_math_deg(f32 radians) 
{
    return radians * (180.0 / M_PI);
}

static inline f32 
fe_math_rad(f32 degrees) 
{
    return degrees * M_PI / 180.0f;
}

static inline i32
fe_math_random_number(i32 min, i32 max)
{
    return (rand() % (max - min + 1)) + min; 
}

static inline f32
fe_math_clamp(f32 num, f32 min, f32 max)
{
    return (num < min) ? min : (max < num) ? max : num;
}
