#pragma once

#include <foam/client/gfx/gpu_mesh.h>

typedef struct
{
    fe_gpu_mesh_instance_id id;
    fe_gpu_mesh_id          gpu_mesh_id;
    fe_color_t              color;
    fe_vec2_t               tex_offset;
    bool                    visible;
} fe_mesh_instance_t;


void fe_mesh_instance_make(fe_mesh_instance_t *ins, fe_gpu_mesh_id gpum_id);
void fe_mesh_instance_kill(fe_mesh_instance_t *ins);
