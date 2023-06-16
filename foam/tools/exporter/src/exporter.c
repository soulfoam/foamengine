#include <foam/core/data/mesh_bundle.h>
#include <foam/core/data/skeleton_data.h>
#include <export_mesh.h>
#include <export_skeleton.h>
#include <exporter.h>
#include <util.h>

static void
write_skeleton(fe_writer_t *writer, const char *path)
{
    fe_skeleton_data_t data;
    if (fe_skeleton_data_make_from_file(&data, path) != FE_OK) return;

    write_data(writer, "\nENTITY\n{\n\tname (\"skeleton\")\n\tskeleton (\"%s\")\n\ttransform ()\n}\n", path);

    fe_vec_for_each(data.bones, b)
    {
        write_data(writer, "\nENTITY\n{\n\tname (\"%s\")\n\tparent (\"skeleton\")\n\ttransform ((0 0 0) (0 0 0) (1 1 1))\n\tbone ()\n}\n", b->name);
    }

    fe_skeleton_data_kill(&data);
}


FE_RETURN_CODE
fe_export_config(const char *path)
{
    fe_config_t cfg;
    if (fe_config_make_from_file(&cfg, path) != FE_OK)
    {
        return FE_ERR_PATH_NOT_FOUND;
    }

    const fe_config_object_t *obj = fe_config_get_object(&cfg, "CONFIG");
    if (!obj)
    {
        fe_log("fe_exporter requires CONFIG object");
        return FE_ERR;
    }

    const fe_value_t *mesh_bundles = fe_config_object_get_value(obj, "mesh_bundles");
    if (mesh_bundles)
    {
        for (u64 i = 0; i < fe_value_get_list_count(mesh_bundles); i++)
        {
            const fe_value_t *list = fe_value_get_list_at(mesh_bundles, i);
            const char *import            = fe_value_as_string(fe_value_get_list_at(list, 0));
            const char *export            = fe_value_as_string(fe_value_get_list_at(list, 1));
            const char *config            = fe_value_as_string(fe_value_get_list_at(list, 2));
            const char *root_entity_name  = fe_value_as_string(fe_value_get_list_at(list, 3));

            if (import && export)
            {
                fe_export_mesh_bundle(import, export, config, root_entity_name);
            }
        }
    }

    const fe_value_t *skeletons = fe_config_object_get_value(obj, "skeletons");
    if (skeletons)
    {
        for (u64 i = 0; i < fe_value_get_list_count(skeletons); i++)
        {
            const fe_value_t *list = fe_value_get_list_at(skeletons, i);
            const char *import            = fe_value_as_string(fe_value_get_list_at(list, 0));
            const char *export            = fe_value_as_string(fe_value_get_list_at(list, 1));
            const char *config            = fe_value_as_string(fe_value_get_list_at(list, 2));

            if (import && export)
            {
                fe_export_skeleton(import, export, config);
            }
        }
    }

 //   write_data(&writer, "%s", "\nRESOURCES\n{\n\tgpu_meshes\n\t(\n");
 //   if (mesh_bundles)
 //   {
 //       for (u64 i = 0; i < fe_value_get_list_count(mesh_bundles); i++)
 //       {
 //           const char *path = fe_value_as_string(fe_value_get_list_at(mesh_bundles, i));
 //           if (path)
 //           {
 //               fe_mesh_bundle_t bundle;
 //               if (fe_mesh_bundle_make_from_file(&bundle, path) != FE_OK) continue;

 //               fe_vec_for_each(fe_mesh_bundle_get_meshes(&bundle), mesh)
 //               {
 //                   write_data(&writer, "\t\t(\"%s\" \"%s\" \"res/missing_texture.png\")\n", mesh->bundle_path, mesh->name);
 //               }
 //               fe_mesh_bundle_kill(&bundle);
 //           }
 //       }
 //   }
 //   write_data(&writer, "%s", "\t)\n}\n");

 //   write_data(&writer, "%s", "# ------------------- #\n");

 //   if (fe_writer_save(&writer, export, "ab+"))
 //   {
 //   	printf("\n--- %s (%lu bytes) successfully exported.\n\n", export, writer.size);
 //   }
 //   else
 //   {
 //   	printf("\n--- %s (%lu bytes) FAILED to export!\n\n", export, writer.size);
 //   }

 //   fe_writer_kill(&writer);

    return FE_OK;
}

