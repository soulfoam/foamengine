#include <foam/core/data/font.h>
#include <foam/client/gfx/renderer.h>
#include <foam/client/ecs/components/button2d.h>

void 
fe_button2d_init(fe_button2d_t *button, const fe_vec2_t *size,
                 fe_button2d_on_down_fn down, fe_button2d_on_up_fn up, fe_button2d_on_hover_fn hover)
{
    fe_aabb_init_from_center_and_size(&button->bounds, &fe_vec3(0, 0, 0), &fe_vec3(size->x, size->y, 0));
    button->down  = down;
    button->up    = up;
    button->hover = hover;
}

