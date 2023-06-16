#include <foam/foam_core.h>
#include <client/ecs/components/debug_manager.h>

static cl_debug_manager_t mngr;

void 
cl_debug_manager_setup(void)
{
    mngr.rigidbodies      = FE_VEC_EMPTY;
    mngr.mesh_colliders   = FE_VEC_EMPTY;
    mngr.simple_colliders = FE_VEC_EMPTY;

    mngr.stats_text_ent        = fe_ecs_add_entity("text");
    fe_text2d_t *text          = fe_entity_add_component(mngr.stats_text_ent, FE_COMPONENT_TEXT2D);
    fe_transform_t *text_xform = fe_entity_add_component(mngr.stats_text_ent, FE_COMPONENT_TRANSFORM);
    fe_text2d_make(text, fe_cache_get_font("res/xga_8x8.png"), fe_renderer_get_resolution(), &FE_COLOR_WHITE, &FE_COLOR_TRANSPARENT);
    fe_transform_init_trs(text_xform, &fe_vec3(0, 0, 0), &FE_VEC3_ZERO, &fe_vec3(text->tex.size.x, text->tex.size.y, 0));

    mngr.show_rigidbodies      = false;
    mngr.show_mesh_colliders   = false;
    mngr.show_simple_colliders = false;

    mngr.text_update_timer = 0;
}

void 
cl_debug_manager_cleanup(void)
{
    fe_vec_kill(mngr.rigidbodies, NULL);
    fe_vec_kill(mngr.mesh_colliders, NULL);
    fe_vec_kill(mngr.simple_colliders, NULL);
    fe_ecs_del_entity(mngr.stats_text_ent);
}

cl_debug_manager_t*
cl_debug_manager_get_ptr(void)
{
    return &mngr;
}

static fe_entity_id
debug_add_aabb(fe_entity_id parent, const fe_aabb_t *aabb)
{
    fe_vec3_t size = fe_aabb_get_size(aabb);

    //fe_mesh_t cube;
    //fe_mesh_make_cube(&cube);
    //mngr.gpu_cube = fe_renderer_add_gpu_mesh(&cube);
    //fe_gpu_mesh_enable_render_flag(fe_renderer_get_gpu_mesh(mngr.gpu_cube), FE_GPU_MESH_RENDER_WIREFRAME);
    //fe_mesh_kill(&cube);

    //fe_entity_id dbg = fe_ecs_add_child_entity(parent, "debug_aabb");
    //fe_mesh_instance_t *ins = fe_entity_add_component(dbg, FE_COMPONENT_MESH_INSTANCE);
    //fe_mesh_instance_make(ins, mngr.gpu_cube);
    //fe_vec3_mul(&size, &fe_vec3(0.5, 0.5, 0.5f));
    //fe_transform_t *xform = fe_entity_add_component(dbg, FE_COMPONENT_TRANSFORM);
    //fe_transform_init_trs(xform, &FE_VEC3_ZERO, &FE_VEC3_ZERO, &size);

    //return dbg;

    return (fe_entity_id){FE_INVALID_ID};
}

void                
cl_debug_manager_enable_rigidbodies(void)
{
    fe_ecs_query(FE_QUERY_RIGIDBODIES, e)
    {
        fe_rigidbody_t *rb = fe_entity_get_component(*e, FE_COMPONENT_RIGIDBODY); 
        fe_transform_t *xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM); 
        fe_aabb_t rb_aabb;
        fe_vec3_t rb_pad = fe_vec3(rb->radius.x * 3, rb->radius.y * 3, rb->radius.z * 3);
        fe_aabb_init_from_center_and_size(&rb_aabb, &FE_VEC3_ZERO, &rb_pad);


        fe_vec_add(mngr.rigidbodies, debug_add_aabb(*e, &rb_aabb));
    }

    mngr.show_rigidbodies = true;
}

void               
cl_debug_manager_enable_mesh_colliders(void)
{
    fe_ecs_query(FE_QUERY_MESH_COLLIDERS, e)
    { 
        fe_mesh_collider_t *mc = fe_entity_get_component(*e, FE_COMPONENT_MESH_COLLIDER);
        fe_vec_add(mngr.mesh_colliders, debug_add_aabb(*e, &mc->aabb));
    }

    mngr.show_mesh_colliders = true;
}

void                
cl_debug_manager_enable_simple_colliders(void)
{
    fe_ecs_query(FE_QUERY_SIMPLE_COLLIDERS, e)
    { 
        fe_simple_collider_t *sc = fe_entity_get_component(*e, FE_COMPONENT_SIMPLE_COLLIDER);
        fe_vec_add(mngr.simple_colliders, debug_add_aabb(*e, &sc->aabb));
    }

    mngr.show_mesh_colliders = true;
}

void                
cl_debug_manager_disable_rigidbodies(void)
{
    fe_vec_for_each(mngr.rigidbodies, rb)
    {
        fe_ecs_del_entity(*rb);
    }

    fe_vec_clear(mngr.rigidbodies, NULL);

    mngr.show_rigidbodies = false;
}

void                
cl_debug_manager_disable_mesh_colliders(void)
{
    fe_vec_for_each(mngr.mesh_colliders, mc)
    {
        fe_ecs_del_entity(*mc);
    }

    fe_vec_clear(mngr.mesh_colliders, NULL);

    mngr.show_mesh_colliders = false;
}

void                
cl_debug_manager_disable_simple_colliders(void)
{
    fe_vec_for_each(mngr.simple_colliders, sc)
    {
        fe_ecs_del_entity(*sc);
    }

    fe_vec_clear(mngr.simple_colliders, NULL);

    mngr.show_simple_colliders = false;
}


