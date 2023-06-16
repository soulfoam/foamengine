#include <foam/client/ecs/ecs_info.h>
#include <foam/client/gfx/window.h>
#include <foam/client/gfx/renderer.h>

void 
fe_client_system_sprite3d_draw(void)
{
    //this is kinda yikes but its fine for now
    //we need to batch glbindtexture calls
    fe_ecs_query(FE_QUERY_SPRITE3D, e)
    {
        fe_sprite3d_t *sprite   = fe_entity_get_component(*e, FE_COMPONENT_SPRITE3D);
        fe_transform_t *xform   = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);

        //fe_mat4x4_t view  = fe_renderer_get_view_matrix();
        //fe_vec3_t view_trl = fe_mat4x4_get_translation(&view);
        //fe_vec3_t world_trl = fe_mat4x4_get_translation(&xform->world_matrix);

        //fe_quat_t rot = fe_quat_look_at(&view_trl, &world_trl);
        //fe_mat4x4_rotate_quat(&xform->world_matrix, &rot);
        //fe_mat4x4_print(&xform->world_matrix);
        
        const fe_mat4x4_t *view = fe_renderer_get_view_matrix();
        fe_mat4x4_t mat;
        fe_mat4x4_identity(&mat);
        fe_vec3_t pos    = fe_mat4x4_get_translation(&xform->world_matrix);
        fe_mat4x4_translate(&mat, &pos);
        mat.m00 = view->m00;
        mat.m01 = view->m10;
        mat.m02 = view->m20;

        mat.m10 = view->m01;
        mat.m11 = view->m11;
        mat.m12 = view->m21;

        mat.m20 = view->m02;
        mat.m21 = view->m12;
        mat.m22 = view->m22;

        fe_mat4x4_rotate_quat(&mat, &xform->rot);
        fe_mat4x4_scale(&mat, &xform->scl);

        fe_gpu_mesh_update_instance_matrix(&sprite->gpu_plane, sprite->id, &mat);
        fe_gpu_mesh_update_instance_color(&sprite->gpu_plane,  sprite->id, &sprite->color);
    }

    fe_ecs_query(FE_QUERY_SPRITE3D, e)
    {
        fe_sprite3d_t *sprite = fe_entity_get_component(*e, FE_COMPONENT_SPRITE3D);
        fe_renderer_draw_gpu_mesh(&sprite->gpu_plane);
    }
}

