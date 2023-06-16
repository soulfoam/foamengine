#pragma once

#include <foam/core/data/mesh.h>
#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/vector.h>
#include <foam/core/ext/hashmap.h>
#include <foam/core/return_code.h>

#define FE_MESH_BUNDLE_VERSION 1

FE_DEFINE_HANDLE(fe_mesh_bundle_id);

typedef struct
{
	char *path;
    fe_vec(fe_mesh_t) meshes;
    fe_hashmap_t      mesh_indexes;
} fe_mesh_bundle_t;

void           fe_mesh_bundle_make(fe_mesh_bundle_t *bundle);
FE_RETURN_CODE fe_mesh_bundle_make_from_file(fe_mesh_bundle_t *bundle, const char *path);
void           fe_mesh_bundle_kill(fe_mesh_bundle_t *bundle);
u32            fe_mesh_bundle_get_count(const fe_mesh_bundle_t *bundle);
fe_mesh_t*     fe_mesh_bundle_get_mesh(const fe_mesh_bundle_t *bundle, const char *mesh_str);
fe_mesh_t*     fe_mesh_bundle_get_mesh_at(const fe_mesh_bundle_t *bundle, u32 index);
fe_mesh_t*     fe_mesh_bundle_get_meshes(const fe_mesh_bundle_t *bundle);
void           fe_mesh_bundle_print(const fe_mesh_bundle_t *bundle);

