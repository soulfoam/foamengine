#pragma once

#include <foam/foam_client.h>
#include <core/core.h>

typedef struct
{
    fe_vec(fe_entity_id) rigidbodies;
    fe_vec(fe_entity_id) mesh_colliders;
    fe_vec(fe_entity_id) simple_colliders;
    fe_entity_id         stats_text_ent;
    fe_gpu_mesh_t        gpu_cube;

    bool show_rigidbodies;
    bool show_mesh_colliders;
    bool show_simple_colliders;

    f32 text_update_timer;
} cl_debug_manager_t;

void                cl_debug_manager_setup(void);
void                cl_debug_manager_cleanup(void);
cl_debug_manager_t* cl_debug_manager_get_ptr(void);
void                cl_debug_manager_enable_rigidbodies(void);
void                cl_debug_manager_enable_mesh_colliders(void);
void                cl_debug_manager_enable_simple_colliders(void);
void                cl_debug_manager_disable_rigidbodies(void);
void                cl_debug_manager_disable_mesh_colliders(void);
void                cl_debug_manager_disable_simple_colliders(void);
