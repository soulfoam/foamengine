#include <foam/core/ecs/components/mesh_collider.h>

void 
fe_mesh_collider_make(fe_mesh_collider_t *mc, const fe_mesh_t *mesh)
{
    mc->mesh = mesh;
    mc->data = fe_mesh_dup(mesh);
    fe_aabb_init_from_mesh(&mc->aabb, mesh);
}

void 
fe_mesh_collider_kill(fe_mesh_collider_t *mc)
{
    fe_mesh_kill(&mc->data);
}

//world space usually
void 
fe_mesh_collider_recalculate(fe_mesh_collider_t *mc, const fe_vec3_t *trl, const fe_quat_t *rot, const fe_vec3_t *scl)
{
    fe_mesh_apply_transform(&mc->data, mc->mesh, trl, rot, scl);
    fe_aabb_init_from_mesh(&mc->aabb, &mc->data);
}
