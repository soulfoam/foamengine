#include <foam/client/resource.h>
#include <foam/client/gfx/renderer.h>
#include <foam/client/ecs/components/sprite3d.h>

void 
fe_sprite3d_make(fe_sprite3d_t *sprite, fe_gpu_texture_id tex_id)
{
    fe_mesh_t plane;
    fe_mesh_make_plane(&plane);
    fe_gpu_mesh_make_from_mesh(&sprite->gpu_plane, &plane);
    fe_gpu_mesh_set_texture_id(&sprite->gpu_plane, tex_id);
    fe_mesh_kill(&plane);

    fe_mat4x4_t mat;
    fe_mat4x4_identity(&mat);
    fe_color_t color    = fe_color_rgb(255, 255, 255);
    sprite->id          = fe_gpu_mesh_add_instance(&sprite->gpu_plane, &mat, &color, &fe_vec2(0, 0));
    sprite->color       = color;
    sprite->billboard   = fe_vec3(0, 0, 0);
}

void 
fe_sprite3d_kill(fe_sprite3d_t *sprite)
{
    fe_gpu_mesh_kill(&sprite->gpu_plane);
}
