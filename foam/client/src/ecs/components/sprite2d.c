#include <foam/client/ecs/components/sprite2d.h>
#include <foam/client/resource.h>
#include <foam/client/gfx/renderer.h>

void 
fe_sprite2d_init(fe_sprite2d_t *sprite, fe_gpu_texture_id tex_id)
{
    sprite->tex_id = tex_id;
    sprite->color  = FE_COLOR_WHITE;
}

const fe_vec2_t* 
fe_sprite2d_get_texture_size(fe_sprite2d_t *sprite)
{
    fe_gpu_texture_t *tex = fe_resource_get_gpu_texture(sprite->tex_id);
    if (!tex) return NULL;

    return &tex->size;
}
