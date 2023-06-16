#include <foam/client/ecs/systems/sys_button2d.h>

void 
fe_client_system_button2d_draw(void)
{
    fe_ecs_query(FE_QUERY_BUTTON2D, e)
    {
        fe_button2d_t *button = fe_entity_get_component(*e, FE_COMPONENT_BUTTON2D);

    }
}
