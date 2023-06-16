#pragma once

#include <foam/core/ecs/ecs.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/vector.h>
#include <foam/core/data/skeleton_data.h>

typedef struct
fe_bone_t
{	
    fe_entity_id skeleton_parent_id;
	const fe_mat4x4_t *offset_matrix;

} fe_bone_t;

void fe_bone_init(fe_bone_t *bone, fe_entity_id skeleton_parent_id, const fe_mat4x4_t *offset_matrix);
