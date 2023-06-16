#include <foam/client/ecs/components/mesh_instance.h>
#include <foam/client/resource.h>

void           
fe_mesh_instance_make(fe_mesh_instance_t *ins, fe_gpu_mesh_id gpum_id)
{
    ins->gpu_mesh_id = gpum_id;
    fe_mat4x4_t mat;
    fe_mat4x4_identity(&mat);
    fe_color_t color = fe_color_rgb(255, 255, 255);
    ins->tex_offset  = fe_vec2(0, 0);
    ins->id          = fe_gpu_mesh_add_instance(fe_resource_get_gpu_mesh(gpum_id), &mat, &color, &ins->tex_offset);
    ins->color       = color;
    ins->visible     = true;
}

void
fe_mesh_instance_kill(fe_mesh_instance_t *ins)
{
    fe_gpu_mesh_del_instance(fe_resource_get_gpu_mesh(ins->gpu_mesh_id), ins->id);
}
