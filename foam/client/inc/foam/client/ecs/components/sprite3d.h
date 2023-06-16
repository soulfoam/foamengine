#pragma once

#include <foam/client/gfx/gpu_texture.h>
#include <foam/client/gfx/gpu_mesh.h>
#include <foam/core/return_code.h>
#include <foam/core/ext/sugar.h>

typedef struct
{ 
    fe_gpu_mesh_t           gpu_plane;
    fe_gpu_mesh_instance_id id;
    fe_color_t              color;
    fe_vec3_t               billboard;
} fe_sprite3d_t;

void fe_sprite3d_make(fe_sprite3d_t *sprite, fe_gpu_texture_id tex_id);
void fe_sprite3d_kill(fe_sprite3d_t *sprite);
