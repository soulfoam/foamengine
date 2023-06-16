#include <foam/core/ext/io.h>
#include <foam/core/ext/logger.h>
#include <foam/core/data/mesh_bundle.h>
#include <foam/core/return_code.h>

void
fe_mesh_bundle_make(fe_mesh_bundle_t *mb)
{
    mb->path   = NULL;
    mb->meshes = FE_VEC_EMPTY;
}

FE_RETURN_CODE
fe_mesh_bundle_make_from_file(fe_mesh_bundle_t *mb, const char *path)
{
    FE_RETURN_CODE rc = FE_OK;
 
    fe_reader_t r;
    rc = fe_reader_make_from_file(&r, path);
    if (rc != FE_OK)
	{
        fe_log_return_code(rc, "fe_mesh_bundle '%s' could not be loaded", path);
        return rc;
	}

    u32 version = fe_io_read_u32(&r);
	if (version != FE_MESH_BUNDLE_VERSION)
	{
        rc = FE_ERR_DATA_VERSION_MISMATCH;
        fe_log_return_code(rc, "fe_mesh_bundle '%s' version mismatch. fe_mesh_bundle version is %u and engine version is %u", path, version, FE_MESH_BUNDLE_VERSION);
        fe_reader_kill(&r);
        return rc;
    }

    fe_hashmap_make(&mb->mesh_indexes);
	
	mb->path       = strdup(path);
    u32 mesh_count = fe_io_read_u32(&r);
    fe_vec_make(mb->meshes, mesh_count);

	for (u32 m = 0; m < mesh_count; m++)
	{
        fe_mesh_t *mesh = NULL;
        fe_vec_add_and_get(mb->meshes, mesh);
       
        char *mesh_name = fe_io_read_string(&r);

        fe_mesh_make(mesh);
        u32 vert_count   = fe_io_read_u32(&r);
        u32 indice_count = fe_io_read_u32(&r);
        fe_vec_make_with_count(mesh->verts, vert_count);
        fe_vec_make_with_count(mesh->indices, indice_count);
        for (u32 i = 0; i < vert_count; i++)
        {
            fe_vertex_t *v = &mesh->verts[i];
            v->pos  = fe_io_read_vec3(&r);
            v->uv   = fe_io_read_vec2(&r);
            v->norm = fe_io_read_vec3(&r);
        }

        for (u32 i = 0; i < indice_count; i++)
        {
            u32 *indice = &mesh->indices[i];
            *indice = fe_io_read_u32(&r);
        }		
        
        fe_hashmap_add(&mb->mesh_indexes, mesh_name, m);
        free(mesh_name);
    }

    fe_reader_kill(&r);
	
    return rc;
}

void
fe_mesh_bundle_kill(fe_mesh_bundle_t *bundle)
{
    fe_vec_kill(bundle->meshes, fe_mesh_kill);
    fe_hashmap_kill(&bundle->mesh_indexes);

	free(bundle->path);
}

u32           
fe_mesh_bundle_get_count(const fe_mesh_bundle_t *bundle)
{
    return fe_vec_get_count(bundle->meshes);
}

fe_mesh_t*
fe_mesh_bundle_get_mesh(const fe_mesh_bundle_t *bundle, const char *mesh_str)
{
    size_t index = fe_hashmap_get(&bundle->mesh_indexes, mesh_str);
    if (index == FE_INVALID_INDEX) return NULL;

    return &bundle->meshes[index];
}

fe_mesh_t*
fe_mesh_bundle_get_meshes(const fe_mesh_bundle_t *bundle)
{
    return bundle->meshes;
}

fe_mesh_t*
fe_mesh_bundle_get_mesh_at(const fe_mesh_bundle_t *bundle, u32 index)
{
    return &bundle->meshes[index];
}

void
fe_mesh_bundle_print(const fe_mesh_bundle_t *bundle)
{
	u32 mesh_count = fe_vec_get_count(bundle->meshes);
	fe_log_debug("Mesh set: (%s) meshes (%u)", bundle->path, mesh_count);

    for (u64 i = 0; i < fe_hashmap_get_capacity(&bundle->mesh_indexes); i++)
    {
        const char *mesh_name = fe_hashmap_get_key_at(&bundle->mesh_indexes, i);
        if (mesh_name)
        {
            fe_mesh_t *mesh = &bundle->meshes[fe_hashmap_get_at(&bundle->mesh_indexes, i)];
            fe_log_debug("Mesh: (%s) ... Vertex count: (%u) ... Indice count: (%u)", 
                mesh_name, fe_vec_get_count(mesh->verts), fe_vec_get_count(mesh->indices));
        }
    }
}

