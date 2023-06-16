#pragma once

#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/vector.h>
#include <foam/core/ext/hashmap.h>
#include <foam/core/return_code.h>

#define FE_VERTS_IN_BYTES(a)   (a * sizeof(fe_vertex_t))
#define FE_INDICES_IN_BYTES(a) (a * sizeof(u32))

#define FE_MESH_PRIMITIVE "fe_mesh_primitive"
#define FE_MESH_PLANE     "plane"
#define FE_MESH_CUBE      "cube"
#define FE_MESH_SPHERE    "sphere"
#define FE_MESH_CYLINDER  "cylinder"

typedef struct
{
	fe_vec3_t pos;
	fe_vec2_t uv;
	fe_vec3_t norm;
} fe_vertex_t;

typedef struct
{
    fe_vec(fe_vertex_t) verts;
    fe_vec(u32)         indices;
} fe_mesh_t;

void      fe_mesh_make(fe_mesh_t *mesh);
void      fe_mesh_kill(fe_mesh_t *mesh);
fe_mesh_t fe_mesh_dup(const fe_mesh_t *mesh);
void      fe_mesh_apply_transform(fe_mesh_t *dst_mesh, const fe_mesh_t *base_mesh, const fe_vec3_t *trl, const fe_quat_t *rot, const fe_vec3_t *scl);

void fe_mesh_make_plane(fe_mesh_t *plane);
void fe_mesh_make_cube(fe_mesh_t *cube);
void fe_mesh_make_sphere(fe_mesh_t *sphere);
void fe_mesh_make_cylinder(fe_mesh_t *mesh);
