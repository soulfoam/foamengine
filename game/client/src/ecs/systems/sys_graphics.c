#include <client/client.h>
#include <foam/foam_client.h>


static f32 spell_rot = 0;

void
cl_system_graphics_tick_texture_scrolling(void)
{
    cl_game_manager_t *gm = cl_game_manager_get_ptr();

    fe_mesh_instance_t *moon_ins = fe_entity_get_component(fe_ecs_find_entity_id(gm->map, "moon"), FE_COMPONENT_MESH_INSTANCE);
    fe_vec2_sub(&moon_ins->tex_offset, &fe_vec2((FE_FT * 0.0001), 0));
    fe_vec2_add(&moon_ins->tex_offset, &fe_vec2(0, (FE_FT * 0.0001)));

    fe_mesh_instance_t *moon_glow = fe_entity_get_component(fe_ecs_find_entity_id(gm->map, "moon_glow"), FE_COMPONENT_MESH_INSTANCE);
    fe_vec2_add(&moon_glow->tex_offset, &fe_vec2((FE_DT * 0.0001), 0.0f));

    fe_mesh_instance_t *blood_pool = fe_entity_get_component(fe_ecs_find_entity_id(gm->map, "blood_pool_cross"), FE_COMPONENT_MESH_INSTANCE);
    fe_vec2_add(&blood_pool->tex_offset, &fe_vec2(0.0f, (FE_DT * 0.0001)));

    fe_mesh_instance_t *skybox = fe_entity_get_component(fe_ecs_find_entity_id(gm->map, "skybox"), FE_COMPONENT_MESH_INSTANCE);
    fe_vec2_add(&skybox->tex_offset, &fe_vec2((FE_DT * 0.00003), 0.0f));

    fe_mesh_instance_t *water = fe_entity_get_component(fe_ecs_find_entity_id(gm->map, "water"), FE_COMPONENT_MESH_INSTANCE);

    water->tex_offset = fe_vec2(sinf(FE_TIME * 0.00001f), cosf(FE_TIME * 0.00001f));

    spell_rot += FE_DT * 0.1f;

   // fe_mesh_instance_t *spellbook = fe_entity_get_component(fe_ecs_find_entity_id(gm->map, "spellbook"), FE_COMPONENT_MESH_INSTANCE);
    fe_transform_t *spellbook_xform = fe_entity_get_component(fe_ecs_find_entity_id(gm->map, "spellbook"), FE_COMPONENT_TRANSFORM);
    //fe_transform_rotate_euler(spellbook_xform, &fe_vec3(0, 45, spell_rot));
    
}
