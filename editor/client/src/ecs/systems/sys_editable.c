#include <client/ecs/systems/sys_editable.h>

static void
update_editable_aabb(fe_entity_id e)
{
    fe_transform_t *xform = fe_entity_get_component(e, FE_COMPONENT_TRANSFORM);
    if (xform)
    {
        ed_editable_t *editable = fe_entity_get_component(e, EDITOR_COMPONENT_EDITABLE);
        fe_mesh_collider_t *mc = fe_entity_get_component(e, FE_COMPONENT_MESH_COLLIDER);
        fe_aabb_t aabb;
        fe_aabb_init_from_center_and_size(&aabb, &fe_vec3(0, 0, 0), &fe_vec3(0, 0, 0));
        if (mc) 
        {
            aabb = mc->aabb;
        }
        editable->aabb = aabb;
    }
}

static void
update_editable_color(fe_entity_id e)
{
    fe_mesh_instance_t *mi = fe_entity_get_component(e, FE_COMPONENT_MESH_INSTANCE); 
    if (!mi) return;

    ed_editable_t *editable = fe_entity_get_component(e, EDITOR_COMPONENT_EDITABLE);
    if (editable->selected)
    {
        mi->color = fe_color_rgb(0, 255, 0);
    }
    else
    {
        mi->color = fe_color_rgb(255, 255, 255);
    }
}

void
ed_system_editable_fixed_tick(void)
{
    ed_manager_t *em = ed_manager_get_ptr();

    fe_ecs_query(EDITOR_QUERY_EDITABLES, e)
    {
        update_editable_aabb(*e);
        update_editable_color(*e);

        ed_editable_t *editable = fe_entity_get_component(*e, EDITOR_COMPONENT_EDITABLE);
        fe_transform_t *xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);

        fe_transform_t *cam_xform = fe_entity_get_component(em->cam, FE_COMPONENT_TRANSFORM);

        fe_ray_t ray;
        fe_transform_get_world_ray(cam_xform, &ray);
        fe_vec3_scale(&ray.dir, editable->dist_from_ray);
        fe_vec3_add(&ray.orig, &ray.dir);

        if (editable->selected)
        {
           // fe_vec3_print(&ray.orig);
           // fe_vec3_print(&editable->orig_pickup_trl);
            fe_transform_set_translation(xform, &ray.orig);
        }

//        fe_ray_t ray;
//        fe_transform_get_world_ray(cam_xform, &ray);
//        fe_vec3_t trl;
//        fe_vec3_add_to(&trl, &ray.orig, &ray.dir);
//        fe_vec3_scale(&trl, editable->dist_from_ray);
//
    }     
}

