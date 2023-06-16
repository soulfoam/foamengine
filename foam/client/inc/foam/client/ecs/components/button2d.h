#pragma once

#include <foam/core/return_code.h>
#include <foam/core/ecs/ecs.h>
#include <foam/core/physics/aabb.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/data/font.h>
#include <foam/client/gfx/gpu_texture.h>

typedef void (*fe_button2d_on_down_fn)(fe_entity_id button_id);
typedef void (*fe_button2d_on_up_fn)(fe_entity_id button_id);
typedef void (*fe_button2d_on_hover_fn)(fe_entity_id button_id);

typedef struct
{  
    fe_aabb_t bounds;

    fe_button2d_on_down_fn  down;
    fe_button2d_on_up_fn    up;
    fe_button2d_on_hover_fn hover;
} fe_button2d_t;

void fe_button2d_init(fe_button2d_t *button, const fe_vec2_t *size, 
                      fe_button2d_on_down_fn down, fe_button2d_on_up_fn up, fe_button2d_on_hover_fn hover
                     );

