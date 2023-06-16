#include <foam/client/ecs/ecs_info.h>
#include <foam/client/resource.h>
#include <foam/client/gfx/renderer.h>

void
fe_client_system_mesh_instance_draw_meshes(void)
{
    fe_renderer_bind_shader(FE_SHADER_INSTANCING);

    fe_ecs_query(FE_QUERY_MESH_INSTANCES, e)
    {
        fe_mesh_instance_t *ins         = fe_entity_get_component(*e, FE_COMPONENT_MESH_INSTANCE);
        fe_transform_t *xform           = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);
        fe_transform_view_t *view_xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM_VIEW);

        if (!ins->visible) continue;

        fe_gpu_mesh_t *gpu_mesh = fe_resource_get_gpu_mesh(ins->gpu_mesh_id);

        const fe_transform_t *world_xform = xform;
        if (view_xform)
        {
            world_xform = fe_transform_view_get_transform(view_xform);
        }

        if (gpu_mesh)
        {
            if (xform->scl.x < 0 || xform->scl.y < 0 || xform->scl.z < 0)
            {
                fe_gpu_mesh_enable_render_flag(gpu_mesh, FE_GPU_MESH_RENDER_DISABLE_CULLING);
            }

            fe_gpu_mesh_update_instance_matrix(gpu_mesh, ins->id, &world_xform->world_matrix);
            fe_gpu_mesh_update_instance_color(gpu_mesh,  ins->id, &ins->color);
            fe_gpu_mesh_update_instance_texture_offset(gpu_mesh, ins->id, &ins->tex_offset);
        }
        else
        {
            fe_log_warn("Mesh '%lu' doesn't exist in the renderer!", ins->gpu_mesh_id);
        }
    }

    fe_renderer_draw_resource_gpu_meshes();
}

