#include <foam/core/core.h>
#include <foam/core/ecs/components/transform_view.h>

void 
fe_transform_view_init(fe_transform_view_t *view_xform)
{
    fe_transform_init(&view_xform->prev_xform);
    fe_transform_init(&view_xform->lerp_xform);
}

const fe_transform_t*
fe_transform_view_get_transform(fe_transform_view_t *view_xform)
{
    return &view_xform->lerp_xform;
}
