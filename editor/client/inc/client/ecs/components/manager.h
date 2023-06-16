#pragma once

#include <foam/foam_client.h>

typedef struct
{
    fe_entity_id root;

    fe_entity_id cam;
    fe_entity_id crosshair;

} ed_manager_t;

void                 ed_manager_setup(void);
void                 ed_manager_cleanup(void);
ed_manager_t*        ed_manager_get_ptr(void);
bool                 ed_manager_add_entity_from_file(const char *path);

