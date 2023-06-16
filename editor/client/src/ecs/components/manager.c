#include <client/editor.h>
#include <foam/foam_client.h>
#include <client/ecs/components/manager.h>

static ed_manager_t mngr;

static void
setup_cam(void)
{
    mngr.cam         = fe_ecs_add_entity("camera");
    ed_camera_t *cam = fe_entity_add_component(mngr.cam, EDITOR_COMPONENT_CAMERA);
    ed_camera_init(cam);

    fe_transform_t *cam_xform = fe_entity_add_component(mngr.cam, FE_COMPONENT_TRANSFORM);
    fe_transform_init_tr(cam_xform, &fe_vec3(0, 0, 0), &fe_vec3(0, 0, 0));

    fe_transform_view_t *cam_view_xform = fe_entity_add_component(mngr.cam, FE_COMPONENT_TRANSFORM_VIEW);
    fe_transform_view_init(cam_view_xform);
}

static void
setup_crosshair(void)
{
    mngr.crosshair        = fe_ecs_add_entity("crosshair");
    fe_sprite2d_t *sprite = fe_entity_add_component(mngr.crosshair, FE_COMPONENT_SPRITE2D);
    fe_transform_t *xform = fe_entity_add_component(mngr.crosshair, FE_COMPONENT_TRANSFORM);

    fe_gpu_texture_id gpu_tex = fe_renderer_add_gpu_texture(fe_cache_get_texture("res/textures/crosshair_0.png"));
    fe_sprite2d_init(sprite, gpu_tex);

    const fe_vec2_t *res = fe_renderer_get_resolution();
    const fe_vec2_t *tex_size = fe_sprite2d_get_texture_size(sprite);
    fe_transform_init_trs(xform, &fe_vec3(res->x/2 - tex_size->x/2, res->y/2 - tex_size->y/2, 0), &FE_VEC3_ZERO, &fe_vec3(tex_size->x, tex_size->y, 0));

}



void 
ed_manager_setup(void)
{
    setup_cam();
    setup_crosshair();

    mngr.root             = fe_ecs_add_entity("scene_root");
    fe_transform_t *xform = fe_entity_add_component(mngr.root, FE_COMPONENT_TRANSFORM);
    fe_transform_init(xform);
}

void 
ed_manager_cleanup(void)
{
   // fe_ecs_del_entity(mngr.cam);
   // fe_ecs_del_entity(mngr.map);
   // fe_ecs_del_entity(mngr.local_player);
}

ed_manager_t*
ed_manager_get_ptr(void)
{
    return &mngr;
}

static void
add_editable(fe_entity_id id)
{
    fe_transform_t *xform = fe_entity_get_component(id, FE_COMPONENT_TRANSFORM);
    if (xform)
    {
        ed_editable_t *editable = fe_entity_add_component(id, EDITOR_COMPONENT_EDITABLE);
        ed_editable_init(editable);

        fe_transform_view_t *xform_view = fe_entity_add_component(id, FE_COMPONENT_TRANSFORM_VIEW);
        fe_transform_view_init(xform_view);
    }

    fe_vec_for_each(fe_entity_get_children(id), c)
    {
        add_editable(*c);
    }
}

bool
ed_manager_add_entity_from_file(const char *path)
{
    fe_entity_id id = fe_ecs_add_entity_from_file(path);
    fe_entity_set_parent(id, mngr.root);

    add_editable(id);

    return FE_VALID_HANDLE_ID(id);
}

