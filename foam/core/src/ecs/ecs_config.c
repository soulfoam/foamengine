#include <foam/core/ecs/ecs_config.h>
#include <foam/core/ecs/ecs_info.h>
#include <foam/core/data/cache.h>

static void
handle_entity_parent(fe_entity_id ent_id, const fe_value_t *val)
{   
    const char *parent_name = fe_value_as_string(fe_value_get_list_at(val, 0));

    fe_entity_id parent_id = fe_entity_get_parent(ent_id);
    fe_entity_id new_parent_id = fe_ecs_find_entity_id(parent_id, parent_name);
    if (!FE_INVALID_HANDLE_ID(new_parent_id))
    {
        fe_entity_set_parent(ent_id, new_parent_id);
    }
}

static void 
handle_entity_transform(fe_entity_id ent_id, const fe_value_t *val)
{ 
    if (fe_value_get_list_count(val) != 3)
    {
        fe_log_warn("transform must contain (trl) (rot) (scl)");
        return;
    }

    fe_vec3_t trl, scl;
    trl = fe_value_as_vec3(fe_value_get_list_at(val, 0));
    scl = fe_value_as_vec3(fe_value_get_list_at(val, 2));

    fe_transform_t *xform = fe_entity_add_component(ent_id, FE_COMPONENT_TRANSFORM);
    fe_transform_init_trs(xform, &trl, &fe_vec3(0, 0, 0), &scl);

    u32 rot_count = fe_value_get_list_count(fe_value_get_list_at(val, 1));
    if (rot_count == 3)
    {
        fe_vec3_t rot = fe_value_as_vec3(fe_value_get_list_at(val, 1));
        fe_quat_t rot_q = fe_quat_from_euler(&rot);
        fe_transform_rotate(xform, &rot_q);
    }
    else if (rot_count == 4)
    {
        fe_vec4_t rot = fe_value_as_vec4(fe_value_get_list_at(val, 1));
        fe_transform_rotate(xform, &fe_quat(rot.x, rot.y, rot.z, rot.w));
    }
}

static void 
handle_entity_transform_view(fe_entity_id ent_id, const fe_value_t *val)
{ 
    fe_transform_view_t *view = fe_entity_add_component(ent_id, FE_COMPONENT_TRANSFORM_VIEW);
    fe_transform_view_init(view);
}

static void 
handle_entity_rigidbody(fe_entity_id ent_id, const fe_value_t *val)
{
    fe_vec3_t size = fe_value_as_vec3(fe_value_get_list_at(val, 0));
    fe_rigidbody_t *rb = fe_entity_add_component(ent_id, FE_COMPONENT_RIGIDBODY);
    fe_rigidbody_init(rb, &size);
}

static void 
handle_entity_mesh_collider(fe_entity_id ent_id, const fe_value_t *val)
{
    const char *mesh_bundle_path = fe_value_as_string(fe_value_get_list_at(val, 0));
    const char *mesh_name        = fe_value_as_string(fe_value_get_list_at(val, 1));
    if (!mesh_bundle_path && !mesh_name) return;

    fe_mesh_bundle_t *mb = fe_cache_get_mesh_bundle(mesh_bundle_path);
    if (!mb) return;
    fe_mesh_t *mesh = fe_mesh_bundle_get_mesh(mb, mesh_name);
    if (!mesh) return;

    fe_mesh_collider_t *collider = fe_entity_add_component(ent_id, FE_COMPONENT_MESH_COLLIDER);
    fe_mesh_collider_make(collider, mesh);

}

static void 
handle_entity_skeleton(fe_entity_id ent_id, const fe_value_t *val)
{ 
    const char *skel_path = fe_value_as_string(fe_value_get_list_at(val, 0));
    if (!skel_path)
    {
        fe_log_error("Entity '%s' must contain skeleton skeleton_data path '%s'", fe_entity_get_name(ent_id), skel_path);
        return;
    }

    fe_skeleton_data_t *sd = fe_cache_get_skeleton_data(skel_path);
    if (!sd)
    {
        fe_log_error("Entity '%s' skeleton failed to load skeleton_data path '%s'", fe_entity_get_name(ent_id), skel_path);
        return;
    }

    fe_skeleton_t *skel = fe_entity_add_component(ent_id, FE_COMPONENT_SKELETON);
    fe_skeleton_make(skel, sd);
}

static void 
handle_entity_bone(fe_entity_id ent_id, const fe_value_t *val)
{
    fe_entity_id skel_id = fe_entity_get_parent(ent_id);
    fe_skeleton_t *skel_comp = NULL;
    while (!FE_INVALID_HANDLE_ID(skel_id))
    {
        skel_comp = fe_entity_get_component(skel_id, FE_COMPONENT_SKELETON);
        if (skel_comp) break;

        skel_id = fe_entity_get_parent(skel_id);
    }

    if (!skel_comp)
    {
        fe_log_error("Entity '%s' bone has no valid skeleton parent. \
                The first parent entity with a skeleton component is the one used.");
        return;
    }

    fe_bone_data_t *bone_data = fe_skeleton_data_get_bone_data(skel_comp->skel_data, fe_entity_get_name(ent_id));
    if (!bone_data)
    {
        fe_log_error("Entity '%s' bone doesn't exist in skeleton_data found on skeleton parent entity '%s'",
                fe_entity_get_name(ent_id), fe_entity_get_name(skel_id));
        return;
    }

    fe_bone_t *bone = fe_entity_add_component(ent_id, FE_COMPONENT_BONE);
    fe_bone_init(bone, skel_id, &bone_data->offset_matrix);
    
}

void 
fe_core_ecs_register_config_callbacks(void)
{
    fe_ecs_register_entity_property_read_callback("parent",        handle_entity_parent);
    fe_ecs_register_entity_property_read_callback("transform",     handle_entity_transform);
    fe_ecs_register_entity_property_read_callback("transform_view",     handle_entity_transform_view);
    fe_ecs_register_entity_property_read_callback("rigidbody",     handle_entity_rigidbody);
    fe_ecs_register_entity_property_read_callback("mesh_collider", handle_entity_mesh_collider);
    fe_ecs_register_entity_property_read_callback("skeleton",      handle_entity_skeleton);
    fe_ecs_register_entity_property_read_callback("bone",          handle_entity_bone);

}
