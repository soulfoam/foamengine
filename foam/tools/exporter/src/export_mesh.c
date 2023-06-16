#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <foam/core/ext/io.h>
#include <foam/core/data/mesh_bundle.h>
#include <export_mesh.h>
#include <util.h>

static const struct aiScene *scene = NULL;
static FileType input_file_type    = OBJ;

static fe_vec(fe_mesh_t) meshes = FE_VEC_EMPTY;

static void
process_mesh(struct aiMesh *am, fe_mesh_t *mesh)
{
    fe_vec_make_with_count(mesh->verts, am->mNumVertices);
    fe_vec_make_with_count(mesh->indices, am->mNumFaces * 3);

	if (!am->mTextureCoords[0])
    {
        fe_log_warn("%s doesn't have tex coords!", mesh->name);
    }

	for (u32 i = 0; i < am->mNumVertices; i++)
	{
		fe_vertex_t *v = &mesh->verts[i];

		v->pos.x = am->mVertices[i].x;
		v->pos.y = am->mVertices[i].y;
		v->pos.z = am->mVertices[i].z;

		if (am->mTextureCoords[0])
		{
			v->uv.x = am->mTextureCoords[0][i].x;
			v->uv.y = am->mTextureCoords[0][i].y;
		}
		else
		{
			v->uv.x = 0.0f;
			v->uv.y = 0.0f;
		}

		v->norm.x = am->mNormals[i].x;
		v->norm.y = am->mNormals[i].y;
		v->norm.z = am->mNormals[i].z;
	}
	
	u32 top = 0;
	for (u32 i = 0; i < am->mNumFaces; i++)
	{
		struct aiFace *f = &am->mFaces[i];

		for (u32 j = 0; j < f->mNumIndices; j++)
        {
            mesh->indices[top++] = f->mIndices[j];
        }
	}

    fe_vec_add(meshes, *mesh);

}

static void
cleanup(void)
{
    fe_vec_kill(meshes, fe_mesh_kill);
    aiReleaseImport(scene);
}

static void
add_root_entity(fe_config_t *cfg, const char *name)
{
    fe_config_object_t *root_obj = fe_config_add_object(cfg, "ROOT_ENTITY");

    fe_value_t *pname = fe_config_object_add_property(root_obj, "name");
    fe_value_add_string_to_list(pname, name);
    fe_value_t *pxform = fe_config_object_add_property(root_obj, "transform");
    fe_value_add_vec3_to_list(pxform, &fe_vec3(0, 0, 0));
    fe_value_add_vec3_to_list(pxform, &fe_vec3(0, 0, 0));
    fe_value_add_vec3_to_list(pxform, &fe_vec3(1, 1, 1));

}

static void
add_entities(fe_config_t *cfg, const char *mesh_bundle_path)
{
    for (size_t i = 0; i < scene->mRootNode->mNumChildren; i++)
    {
        //add support for all meshes? usually is just 1 always i think?
        struct aiNode *node = scene->mRootNode->mChildren[i];
        if (node->mNumMeshes <= 0) continue;

        fe_config_object_t *obj = fe_config_add_object(cfg, "ENTITY");

        fe_value_t *pname = fe_config_object_add_property(obj, "name");
        fe_value_add_string_to_list(pname, node->mName.data);

        fe_vec3_t trl = fe_vec3(0, 0, 0);
        fe_quat_t rot = fe_quat(0, 0, 0, 1);
        fe_vec3_t scl = fe_vec3(1, 1, 1);
        aiMat4_decompose(&node->mTransformation, &trl, &rot, &scl);
        fe_value_t *pxform = fe_config_object_add_property(obj, "transform");
        fe_value_add_vec3_to_list(pxform, &trl);
        fe_value_add_vec4_to_list(pxform, &fe_vec4(rot.x, rot.y, rot.z, rot.w));
        fe_value_add_vec3_to_list(pxform, &scl);

        fe_value_t *pmesh_instance = fe_config_object_add_property(obj, "mesh_instance");
        fe_value_add_string_to_list(pmesh_instance, mesh_bundle_path);
        fe_value_add_string_to_list(pmesh_instance, scene->mMeshes[node->mMeshes[0]]->mName.data);
    }

}

static void
add_resources(fe_config_t *cfg, const fe_mesh_bundle_t *mb)
{ 
    fe_config_object_t *res_obj   = fe_config_add_object(cfg, "RESOURCES");
    fe_value_t *gpu_meshes = fe_config_object_add_property(res_obj, "gpu_meshes");

    fe_vec_for_each(fe_mesh_bundle_get_meshes(mb), mesh)
    {
        fe_value_t v;
        fe_value_make_list(&v);
        fe_value_add_string_to_list(&v, mesh->bundle_path);
        fe_value_add_string_to_list(&v, mesh->name);
        fe_value_add_string_to_list(&v, "res/textures/missing_texture.png");
        fe_value_add_value_to_list(gpu_meshes, &v);
        fe_value_kill(&v);
    }
}



static bool
gpu_meshes_contains(fe_value_t *gpu_meshes, const char *bundle, const char *name)
{
    fe_vec_for_each(gpu_meshes->list, l)
    {
        const char *b = fe_value_as_string(fe_value_get_list_at(l, 0)); 
        const char *n = fe_value_as_string(fe_value_get_list_at(l, 1)); 
        if (b && n && STR_EQ(b, bundle) && STR_EQ(n, name))
        {
            return true; 
        }
    }

    return false;
}

static void
update_resources_object(fe_config_object_t *dst, fe_config_object_t *other)
{   
    fe_value_t *gpu_meshes_dst   = fe_config_object_get_value(dst, "gpu_meshes");
    fe_value_t *gpu_meshes_other = fe_config_object_get_value(other, "gpu_meshes");

    if (gpu_meshes_dst && gpu_meshes_other)
    {     
        fe_vec_for_each(gpu_meshes_other->list, ov_list)
        {
            const char *bundle = fe_value_as_string(fe_value_get_list_at(ov_list, 0)); 
            const char *name   = fe_value_as_string(fe_value_get_list_at(ov_list, 1)); 
            if (!gpu_meshes_contains(gpu_meshes_dst, bundle, name))
            {
                fe_value_add_value_to_list(gpu_meshes_dst, ov_list); 
            }
        }
    }
}

static void
export_config(const char *mesh_bundle_path, const char *config_path, const char *root_ent_name)
{
    fe_mesh_bundle_t bundle;
    if (fe_mesh_bundle_make_from_file(&bundle, mesh_bundle_path) != FE_OK) return;

    fe_config_t cfg;
    fe_config_make(&cfg);

    add_root_entity(&cfg, root_ent_name);
    add_entities(&cfg, mesh_bundle_path);
    add_resources(&cfg, &bundle);

    fe_config_t prev_cfg;
    if (fe_config_make_from_file(&prev_cfg, config_path) == FE_OK)
    {
        config_update_entities(&prev_cfg, &cfg);
        config_add_missing_entities(&prev_cfg, &cfg);

        fe_config_object_t *prev_res_obj = fe_config_get_object(&prev_cfg, "RESOURCES");
        fe_config_object_t *new_res_obj  = fe_config_get_object(&cfg, "RESOURCES");
        if (prev_res_obj && new_res_obj)
        {
            update_resources_object(prev_res_obj, new_res_obj);
        }

        fe_config_save(&prev_cfg, config_path);
    }
    else
    {
        fe_config_save(&cfg, config_path);
    }

    fe_config_kill(&cfg);
    fe_mesh_bundle_kill(&bundle);

}

static bool
export_bundle(const char *path)
{	
    fe_writer_t writer;
    fe_writer_make(&writer);
    
    fe_io_write_u32(&writer, FE_MESH_BUNDLE_VERSION);
    fe_io_write_u32(&writer, fe_vec_get_count(meshes));

    for (u32 i = 0; i < fe_vec_get_count(meshes); i++)
    {
        fe_mesh_t *md = &meshes[i];
        u32 vert_count = fe_vec_get_count(md->verts);
        u32 indice_count = fe_vec_get_count(md->indices);
        fe_io_write_string(&writer, md->name);
        fe_io_write_u32(&writer, vert_count);
        fe_io_write_u32(&writer, indice_count);
        for (u32 vc = 0; vc < vert_count; vc++)
        {
            fe_vertex_t *v = &md->verts[vc];

            fe_io_write_vec3(&writer, &v->pos);
            fe_io_write_vec2(&writer, &v->uv);
            fe_io_write_vec3(&writer, &v->norm);
        }
        for (u32 ic = 0; ic < indice_count; ic++)
        {
            fe_io_write_u32(&writer, md->indices[ic]);
        }
    }

    bool success = true;
	if (fe_writer_save(&writer, path, "wb"))
	{
		printf("\n\n'%s' (%lu bytes) successfully exported.\n", path, writer.size);
	}
	else
	{
		printf("\n\n'%s' (%lu bytes) FAILED to export!\n", path, writer.size);
        success = false;
	}
	
	fe_writer_kill(&writer);

    return success;
}

void 
fe_export_mesh_bundle(const char *input_path, const char *output_path, const char *config_path, const char *root_ent_name)		
{	

	scene = aiImportFile(input_path, aiProcessPreset_TargetRealtime_MaxQuality);

	input_file_type = get_file_type(input_path);

	if (!scene)
	{
		fe_log("Failed to load file %s\n%s\n", input_path, aiGetErrorString());
		return;
	}

	for (u32 i = 0; i < scene->mNumMeshes; i++)
	{
		struct aiMesh *am = scene->mMeshes[i];
        
		fe_mesh_t mesh;
        fe_mesh_make(&mesh, NULL, am->mName.data);

	//	if (input_file_type == OBJ)
	//	{
	//		char *n_name = strrchr(mesh.name, '_');
	//		if (n_name) *n_name = '\0';
	//	}

		process_mesh(am, &mesh);

	}

    printf("\n---- fe_mesh_export ----\n\n");

    fe_vec_for_each_index(meshes, m, i)
    {
        printf("%lu: '%s' (vertex_count: %lu) (indice_count: %lu)\n", 
                i, m->name, fe_vec_get_count(m->verts), fe_vec_get_count(m->indices));
    }

	bool exported = export_bundle(output_path);

    printf("---- -------------- ----\n\n");

    if (exported && config_path && root_ent_name)
    {
        export_config(output_path, config_path, root_ent_name);
    }

	cleanup();
}

