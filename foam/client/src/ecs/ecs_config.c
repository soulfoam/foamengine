#include <foam/core/data/cache.h>
#include <foam/client/resource.h>
#include <foam/client/ecs/ecs_info.h>
#include <foam/client/ecs/ecs_config.h>
#include <foam/client/gfx/renderer.h>

static void
handle_resource_gpu_meshes(const fe_value_t *val)
{
    for (u64 i = 0; i < fe_value_get_list_count(val); i++)
    {
        const fe_value_t *list = fe_value_get_list_at(val, i);
        if (fe_value_get_list_count(list) != 3)
        {
            fe_log_warn("RESOURCES 'gpu_meshes' must contain (mesh_bundle_path mesh_name texture_path)");
            continue;
        }

        const char *mesh_bundle_path = fe_value_as_string(fe_value_get_list_at(list, 0));
        const char *mesh_name        = fe_value_as_string(fe_value_get_list_at(list, 1));
        const char *texture_path     = fe_value_as_string(fe_value_get_list_at(list, 2));

        fe_mesh_bundle_t *bundle = fe_cache_get_mesh_bundle(mesh_bundle_path);
        if (!bundle) continue;
        fe_mesh_t *mesh          = fe_mesh_bundle_get_mesh(bundle, mesh_name);
        if (!mesh) continue;
        fe_texture_t *tex        = fe_cache_get_texture(texture_path);

        fe_gpu_texture_id tex_id = fe_resource_get_gpu_texture_id(texture_path);
        if (FE_INVALID_HANDLE_ID(tex_id))
        {
            if (tex) 
            {
                fe_gpu_texture_t *gpu_tex = NULL;
                tex_id = fe_resource_add_gpu_texture(&gpu_tex);
                fe_resource_tag_gpu_texture(tex_id, texture_path);
                fe_gpu_texture_make_from_texture(gpu_tex, tex);
            }
        }

        fe_gpu_mesh_id gpu_mesh_id = fe_resource_get_gpu_mesh_id(mesh_bundle_path, mesh_name);
        if (FE_INVALID_HANDLE_ID(gpu_mesh_id))
        {
            fe_gpu_mesh_t *gpu_mesh = NULL;
            gpu_mesh_id = fe_resource_add_gpu_mesh(&gpu_mesh);
            fe_resource_tag_gpu_mesh(gpu_mesh_id, mesh_bundle_path, mesh_name);
            fe_gpu_mesh_make_from_mesh(gpu_mesh, mesh);
            fe_gpu_mesh_set_texture_id(gpu_mesh, tex_id);
        }

    }
}

static void
handle_resource_gpu_mesh_flags(const fe_value_t *val)
{
    for (u64 i = 0; i < fe_value_get_list_count(val); i++)
    {
        const fe_value_t *list = fe_value_get_list_at(val, i);
        if (fe_value_get_list_count(list) != 3)
        {
            fe_log_warn("RESOURCES 'gpu_mesh_flags' must contain (mesh_bundle_path mesh_name flags)");
            continue;
        }

        const char *mesh_bundle_path = fe_value_as_string(fe_value_get_list_at(list, 0));
        const char *mesh_name        = fe_value_as_string(fe_value_get_list_at(list, 1));
        const char *flags            = fe_value_as_string(fe_value_get_list_at(list, 2));


        fe_gpu_mesh_id gpu_mesh_id = fe_resource_get_gpu_mesh_id(mesh_bundle_path, mesh_name);
        fe_gpu_mesh_t *gpu_mesh    = fe_resource_get_gpu_mesh(gpu_mesh_id);
        if (FE_INVALID_HANDLE_ID(gpu_mesh_id))
        {
            fe_log_error("gpu_mesh_flags invalid gpu_mesh_id '%s' '%s'", mesh_bundle_path, mesh_name);
            continue;
        }

        if (STR_EQ(flags, "DISABLE_CULLING"))
        {
            fe_gpu_mesh_enable_render_flag(gpu_mesh, FE_GPU_MESH_RENDER_DISABLE_CULLING);
        }
    }
}

static void
handle_entity_mesh_instance(fe_entity_id ent_id, const fe_value_t *val)
{ 
    if (fe_value_get_list_count(val) != 2)
    {
        fe_log_warn("mesh_instance must contain 'mesh_bundle_path' && 'mesh_name'");
        return;
    }

    const char *mesh_bundle_path = fe_value_as_string(fe_value_get_list_at(val, 0));
    const char *mesh_name        = fe_value_as_string(fe_value_get_list_at(val, 1));

    fe_gpu_mesh_id gpu_mesh_id = fe_resource_get_gpu_mesh_id(mesh_bundle_path, mesh_name);
    if (FE_INVALID_HANDLE_ID(gpu_mesh_id))
    {
        fe_log_error("mesh_instance invalid gpu_mesh_id '%s' '%s'", mesh_bundle_path, mesh_name);
        return;
    }

    fe_mesh_instance_t *ins = fe_entity_add_component(ent_id, FE_COMPONENT_MESH_INSTANCE);
    fe_mesh_instance_make(ins, gpu_mesh_id);
}

void
fe_client_register_config_callbacks(void)
{
    fe_ecs_register_resource_property_read_callback("gpu_meshes", handle_resource_gpu_meshes);
    fe_ecs_register_resource_property_read_callback("gpu_mesh_flags", handle_resource_gpu_mesh_flags);
    fe_ecs_register_entity_property_read_callback("mesh_instance", handle_entity_mesh_instance);
}

