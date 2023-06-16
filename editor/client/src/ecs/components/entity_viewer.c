#include <client/editor.h>
#include <client/ecs/components/entity_viewer.h>

static fe_entity_viewer_t ev;

void 
ed_entity_viewer_setup(void)
{
    ev.viewer = fe_ecs_add_entity("entity_viewer");
    fe_sprite2d_t *bg_s = fe_entity_add_component(ev.viewer, FE_COMPONENT_SPRITE2D);

    ev.entity_items = NULL;
}

void 
ed_entity_viewer_cleanup(void)
{
    fe_vec_kill(ev.entity_items, NULL);
}

