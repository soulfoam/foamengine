#include <foam/core/core.h>
#include <foam/core/ecs/ecs_info.h>

void 
fe_core_system_transform_view_update_previous_fixed(void)
{
    fe_ecs_query(FE_QUERY_TRANSFORM_VIEWS, e)
    {
        fe_transform_t *xform           = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);
        fe_transform_view_t *view_xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM_VIEW);
        view_xform->prev_xform = *xform;
    }
}

void 
fe_core_system_transform_view_generate_world_matrix(void)
{
    fe_ecs_query(FE_QUERY_TRANSFORM_VIEWS, e)
    {
        fe_transform_t *xform           = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);
        fe_transform_view_t *view_xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM_VIEW); 

        fe_vec3_lerp_to(&view_xform->lerp_xform.world_trl,  &view_xform->prev_xform.world_trl, &xform->world_trl, FE_FT);
        fe_quat_slerp_to(&view_xform->lerp_xform.world_rot, &view_xform->prev_xform.world_rot, &xform->world_rot, FE_FT);
        fe_vec3_lerp_to(&view_xform->lerp_xform.world_scl,  &view_xform->prev_xform.world_scl, &xform->world_scl, FE_FT);

        fe_transform_generate_world_matrix(&view_xform->lerp_xform, &view_xform->lerp_xform.world_matrix);
    }
}

