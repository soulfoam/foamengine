#pragma once

#include <foam/core/ecs/ecs.h>

typedef struct
{
    fe_vec(fe_entity_id) entity_items;
    fe_entity_id viewer;

} fe_entity_viewer_t;

void ed_entity_viewer_setup(void);
void ed_entity_viewer_cleanup(void);
