#include <foam/client/ecs/ecs_info.h>

//static const char *SIMPLE_COLLISION_MASK[] =
//{
//    FE_COMPONENT_SIMPLE_COLLIDER, 
//    FE_COMPONENT_TRANSFORM
//};
//
//static const char *MESH_COLLIDER_MASK[] =
//{
//    FE_COMPONENT_MESH_COLLIDER, 
//    FE_COMPONENT_TRANSFORM
//};

static bool r_simple_col = false;

static void
render_simple_colliders(void)
{
    //if (fe_input_is_key_down(FE_KEY_0)) r_simple_col = !r_simple_col;

    //if (!r_simple_col) return;

    //FE_GpuMesh *cube = fe_renderer_get_gpu_mesh("cube");
    //for (u32 i = 0; i < fe_ecs_get_entity_count(); i++)
    //{
    //    FE_Entity *e = fe_ecs_get_entity_at(i);
    //    if (fe_entity_has_mask(e, FE_MASK(SIMPLE_COLLISION_MASK)))
    //    {             
    //    
    //        //FE_Rigidbody *rb = fe_entity_get_component(e, FE_COMPONENT_RIGIDBODY); 
    //        FE_SimpleCollider *sc = fe_entity_get_component(e, FE_COMPONENT_SIMPLE_COLLIDER);
    //        FE_Transform *xform = fe_entity_get_component(e, FE_COMPONENT_TRANSFORM);
    //        //fe_debug_manager_add_model(debug, model, rb->radius);
    //        
    //        for (u32 j = 0; j < fe_simple_collider_get_aabb_count(sc); j++)
    //        {
    //            const FE_AABB *aabb = fe_simple_collider_get_aabb_at(sc, j);
    //            FE_MeshInstance ins;
    //            fe_mesh_instance_init(&ins, "cube");

    //            mat4x4_translate_self(ins.xform, xform->pos);
    //            mat4x4_scale_self(ins.xform, aabb->size);

    //            fe_gpu_mesh_push_instance(cube, &ins);

    //            fe_mesh_instance_destroy(&ins);
    //        }
    //    } 
    //}

    //for (u32 i = 0; i < fe_ecs_get_entity_count(); i++)
    //{
    //    FE_Entity *e = fe_ecs_get_entity_at(i);
    //    if (fe_entity_has_mask(e, FE_MASK(MESH_COLLIDER_MASK)))
    //    {             
    //    
    //        //FE_Rigidbody *rb = fe_entity_get_component(e, FE_COMPONENT_RIGIDBODY); 
    //        FE_MeshCollider *mc = fe_entity_get_component(e, FE_COMPONENT_MESH_COLLIDER);
    //        FE_Transform *xform = fe_entity_get_component(e, FE_COMPONENT_TRANSFORM);
    //        //fe_debug_manager_add_model(debug, model, rb->radius);
    //        
    //        for (u32 j = 0; j < fe_mesh_collider_get_mesh_count(mc); j++)
    //        {
    //            FE_AABB aabb = fe_mesh_collider_get_mesh_aabb_at(mc, j);
    //            const FE_Mesh *mesh = fe_mesh_collider_get_mesh_at(mc, j);
    //            
    //            FE_MeshInstance ins;
    //            fe_mesh_instance_init(&ins, "cube");

    //            mat4x4_translate_self(ins.xform, xform->pos);
    //            mat4x4_translate_self(ins.xform, aabb.center);
    //            mat4x4_scale_self(ins.xform, aabb.size);

    //            fe_gpu_mesh_push_instance(cube, &ins);

    //            fe_mesh_instance_destroy(&ins);
    //        }
    //    } 
    //}

}

void 
fe_client_register_debug_systems(void)
{ 
  //  fe_ecs_register_system(render_simple_colliders, FE_ECS_SYSTEM_TYPE_RENDER);
}


