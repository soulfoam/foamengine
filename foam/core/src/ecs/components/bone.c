#include <foam/core/ecs/components/bone.h>

void
fe_bone_init(fe_bone_t *bone, fe_entity_id skeleton_parent_id, const fe_mat4x4_t *offset_matrix)
{
    bone->skeleton_parent_id = skeleton_parent_id;
    bone->offset_matrix      = offset_matrix;
}
