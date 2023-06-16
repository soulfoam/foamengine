#include <client/ecs/components/game_manager.h>

static cl_game_manager_t mngr;

static void
setup_cam(void)
{
    mngr.cam             = fe_ecs_add_entity("camera");
    cl_fps_camera_t *cam = fe_entity_add_component(mngr.cam, CL_COMPONENT_FPS_CAMERA);
    cl_fps_camera_init(cam);

    fe_transform_t *cam_xform = fe_entity_add_component(mngr.cam, FE_COMPONENT_TRANSFORM);
    fe_transform_init_tr(cam_xform, &fe_vec3(0, 0, 1.25f), &fe_vec3(0, 0, 0));

    fe_entity_set_parent(mngr.cam, fe_ecs_find_entity_id(mngr.local_player, "camera_attachment"));

    fe_transform_view_t *cam_view_xform = fe_entity_add_component(mngr.cam, FE_COMPONENT_TRANSFORM_VIEW);
    fe_transform_view_init(cam_view_xform);
    
    fe_entity_id wep = fe_ecs_add_child_entity(mngr.cam, "weapon_attachment");

    fe_transform_t *wep_xform = fe_entity_add_component(wep, FE_COMPONENT_TRANSFORM);
    fe_transform_init_t(wep_xform, &fe_vec3(0, 1, -1));

}

static void
setup_crosshair(void)
{
    mngr.crosshair        = fe_ecs_add_entity("crosshair");
    fe_sprite2d_t *sprite = fe_entity_add_component(mngr.crosshair, FE_COMPONENT_SPRITE2D);
    fe_transform_t *xform = fe_entity_add_component(mngr.crosshair, FE_COMPONENT_TRANSFORM);

    fe_gpu_texture_t *gpu_tex = NULL;
    fe_gpu_texture_id tex_id = fe_resource_add_gpu_texture(&gpu_tex);
    fe_gpu_texture_make_from_texture(gpu_tex, fe_cache_get_texture("res/textures/crosshair_0.png"));
    fe_sprite2d_init(sprite, tex_id);

    const fe_vec2_t *res = fe_renderer_get_resolution();
    const fe_vec2_t *tex_size = fe_sprite2d_get_texture_size(sprite);
    fe_transform_init_trs(xform, &fe_vec3(res->x/2 - tex_size->x/2, res->y/2 - tex_size->y/2, 0), &FE_VEC3_ZERO, &fe_vec3(tex_size->x, tex_size->y, 0));

}

static void
setup_player(void)
{
    mngr.local_player = fe_ecs_add_entity_from_file("res/configs/player.feco");
    co_player_input_t *pi = fe_entity_add_component(mngr.local_player, CO_COMPONENT_PLAYER_INPUT);
    co_player_input_init(pi);
}

static void
setup_weapon(void)
{
    mngr.weapon           = fe_ecs_add_entity_from_file("res/configs/rocket_launcher.feco");
    fe_entity_set_parent(mngr.weapon, mngr.local_player);

}

void 
cl_game_manager_setup(void)
{
    setup_player();
    setup_cam();
    setup_crosshair();

    mngr.map          = fe_ecs_add_entity_from_file("res/configs/map.feco");

    fe_entity_id lol = fe_ecs_add_entity_from_file("res/configs/knight.feco");
    fe_skeleton_t *skel = fe_entity_get_component(fe_ecs_find_entity_id(lol, "Armature"), FE_COMPONENT_SKELETON);
    //fe_skeleton_play_animation(skel, "stance", true);

    fe_simple_collider_t *sc = fe_entity_add_component(mngr.local_player, FE_COMPONENT_SIMPLE_COLLIDER);

    fe_aabb_t aabb;
    fe_aabb_init(&aabb, &fe_vec3(0, 0, -1), &fe_vec3(1, 1, 1));
    fe_simple_collider_make(sc, &aabb, NULL, NULL, NULL);


    setup_weapon();


}

void 
cl_game_manager_cleanup(void)
{
   // fe_ecs_del_entity(mngr.cam);
   // fe_ecs_del_entity(mngr.map);
   // fe_ecs_del_entity(mngr.local_player);
}

cl_game_manager_t*
cl_game_manager_get_ptr(void)
{
    return &mngr;
}


