#pragma once

#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/physics/ray.h>
#include <foam/core/ecs/components/transform.h>

typedef struct
{
    fe_transform_t prev_xform;
    fe_transform_t lerp_xform;
} fe_transform_view_t;

void  fe_transform_view_init(fe_transform_view_t *view_xform);
const fe_transform_t *fe_transform_view_get_transform(fe_transform_view_t *view_xform);

