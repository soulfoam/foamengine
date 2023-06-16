#include <foam/core/core.h>
#include <foam/core/ecs/ecs_info.h>
#include <foam/core/ext/logger.h>

void
fe_core_system_skeleton_move_bones(void)
{
    fe_ecs_query(FE_QUERY_BONES, e)
	{

        fe_bone_t *bone       = fe_entity_get_component(*e, FE_COMPONENT_BONE);
        fe_transform_t *xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);
        fe_skeleton_t *skel   = fe_entity_get_component(bone->skeleton_parent_id, FE_COMPONENT_SKELETON);

        if (!skel || !skel->anim.cur_anim_data) continue;

        const char *name = fe_entity_get_name(*e);
        const fe_bone_animation_data_t *bone_anim = fe_skeleton_animation_data_get_bone_animation_data
                                                    (skel->anim.cur_anim_data, name);

        if (skel->anim.playing)
        {
            fe_transform_set_translation(xform, &bone_anim->translations[skel->anim.cur_frame]);
            fe_transform_set_rotation(xform, &bone_anim->rotations[skel->anim.cur_frame]);
            fe_transform_set_scale(xform, &bone_anim->scales[skel->anim.cur_frame]);
        }

	}

}

void
fe_core_system_skeleton_apply_bone_offset_matrix(void)
{
    fe_ecs_query(FE_QUERY_BONES, e)
	{ 
        fe_bone_t *bone       = fe_entity_get_component(*e, FE_COMPONENT_BONE);
        fe_transform_t *xform = fe_entity_get_component(*e, FE_COMPONENT_TRANSFORM);
        fe_skeleton_t *skel   = fe_entity_get_component(bone->skeleton_parent_id, FE_COMPONENT_SKELETON);
        if (!skel || !skel->anim.playing) continue;

        fe_mat4x4_mul(&xform->world_matrix, bone->offset_matrix);
	}

}

void
fe_core_system_skeleton_tick_animation(void)
{
    fe_ecs_query(FE_QUERY_SKELETONS, e)
    {
        fe_skeleton_t *skel = fe_entity_get_component(*e, FE_COMPONENT_SKELETON);
        if (skel->anim.playing)
        {
            f32 time_per_frame = 1000.0f / skel->anim.tick_rate;
            skel->anim.time += FE_DT / time_per_frame;

            while (skel->anim.time >= 1.0f)
            {
                skel->anim.cur_frame++;
                skel->anim.time--;
            }

            skel->anim.next_frame = skel->anim.cur_frame+1; 
            if (skel->anim.next_frame >= skel->anim.duration)
            {
                if (skel->anim.loop)
                {
                    skel->anim.next_frame = 0;
                }
                else 
                {
                    skel->anim.next_frame = skel->anim.duration-1;
                }
            }

            if (skel->anim.cur_frame >= skel->anim.duration)
            {
                if (skel->anim.loop)
                {
                    skel->anim.cur_frame = 0;
                    skel->anim.next_frame = 1;
                }
                else
                {
                    skel->anim.cur_frame = skel->anim.duration-1;
                    skel->anim.playing = false;
                }
                
                skel->anim.played_once = true;
            }
     
        }
    }

}


