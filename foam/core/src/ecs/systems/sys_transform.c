#include <foam/core/core.h>
#include <foam/core/ecs/ecs_info.h>

static void
generate_world_matrix(fe_entity_id e)
{
    fe_transform_t *xform   = fe_entity_get_component(e, FE_COMPONENT_TRANSFORM);
    fe_bone_t *bone         = fe_entity_get_component(e, FE_COMPONENT_BONE);
    fe_transform_t *p_xform = fe_entity_get_component(fe_entity_get_parent(e), FE_COMPONENT_TRANSFORM);
    fe_bone_t *p_bone       = fe_entity_get_component(fe_entity_get_parent(e), FE_COMPONENT_BONE);

    //bones need calculated on their own
    if (bone || p_bone)
    {
        if (p_xform)
        {
            fe_mat4x4_t loc;
            fe_transform_generate_matrix(xform, &loc);
            fe_mat4x4_mul_to(&xform->world_matrix, &p_xform->world_matrix, &loc);
        }
        else
        {    
            fe_transform_generate_world_matrix(xform, &xform->world_matrix);
        }
    }
    else
    {
        fe_transform_generate_world_matrix(xform, &xform->world_matrix);
    }

}

static bool
should_generate(fe_transform_t *xform, fe_transform_t *p_xform)
{
    if (xform->flags & FE_TRANSFORM_DIRTY_LOCAL)
        return true;

    if (p_xform && (p_xform->flags & FE_TRANSFORM_DIRTY_LOCAL))
        return true;

    return false;
}

void 
fe_core_system_transform_generate_world_transform_fixed(void)
{
    fe_ecs_query(FE_QUERY_TRANSFORMS, e)
    {
        fe_transform_t *xform   = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);
        fe_transform_t *p_xform = fe_entity_get_component(fe_entity_get_parent(*e), FE_COMPONENT_TRANSFORM);

        bool gen = should_generate(xform, p_xform);

        if (gen)
        {

            xform->world_trl = xform->trl;
            xform->world_rot = xform->rot;
            xform->world_scl = xform->scl;

            if (p_xform)
            {
                if (!(xform->flags & FE_TRANSFORM_IGNORE_PARENT_TRL))
                fe_vec3_add_to(&xform->world_trl, &xform->trl, &p_xform->world_trl);
                if (!(xform->flags & FE_TRANSFORM_IGNORE_PARENT_ROT))
                fe_quat_mul_to(&xform->world_rot, &xform->rot, &p_xform->world_rot);
                if (!(xform->flags & FE_TRANSFORM_IGNORE_PARENT_SCL))
                fe_vec3_mul_to(&xform->world_scl, &xform->scl, &p_xform->world_scl);
            }

            generate_world_matrix(*e);

            //maybe make callbacks for the recalcs instead?
            fe_mesh_collider_t *mc = fe_entity_get_component(*e, FE_COMPONENT_MESH_COLLIDER);
            if (mc)
            {
                fe_mesh_collider_recalculate(mc, &xform->world_trl, &xform->world_rot, &xform->world_scl);
            }
            /////////
        }
    }


    fe_ecs_query(FE_QUERY_TRANSFORMS, e)
    {
        fe_transform_t *xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);
        xform->flags &= ~FE_TRANSFORM_DIRTY_LOCAL;
    }
        
}


