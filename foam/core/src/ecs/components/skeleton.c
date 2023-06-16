#include <string.h>
#include <assert.h>
#include <foam/core/ecs/ecs_info.h>
#include <foam/core/ext/logger.h>

static void
reset_anim(fe_skeleton_t *skel)
{
	skel->anim.playing       = false;
	skel->anim.played_once   = false;
	skel->anim.loop          = false;
	skel->anim.time          = 0;
	skel->anim.cur_frame     = 0;
	skel->anim.next_frame    = 0;
	skel->anim.cur_anim_data = NULL;
	skel->anim.duration      = 0;
	skel->anim.tick_rate     = 0;
}

void 
fe_skeleton_make(fe_skeleton_t *skel, const fe_skeleton_data_t *skel_data)
{
	skel->skel_data = skel_data;
	reset_anim(skel);
}

FE_RETURN_CODE 
fe_skeleton_make_with_bones(fe_skeleton_t *skel, const fe_skeleton_data_t *skel_data, fe_entity_id parent_id, const char *skeleton_parent_name)
{
    FE_RETURN_CODE rc = FE_OK;

	skel->skel_data = skel_data;
	reset_anim(skel);

    skel->skeleton_id = fe_ecs_add_child_entity(parent_id, skeleton_parent_name);
    fe_transform_t *skel_xform = fe_entity_add_component(skel->skeleton_id, FE_COMPONENT_TRANSFORM);
    fe_transform_init_t(skel_xform, &fe_vec3(0, 0, 0));

    fe_vec_for_each(skel_data->bones, bone_data)
    {
        fe_entity_id bone_id = fe_ecs_add_child_entity(skel->skeleton_id, bone_data->name);

        fe_bone_t *bone = fe_entity_add_component(bone_id, FE_COMPONENT_BONE);
        fe_bone_init(bone, parent_id, &bone_data->offset_matrix);

        fe_transform_t *xform = fe_entity_add_component(bone_id, FE_COMPONENT_TRANSFORM);
        fe_transform_init(xform);
    }

    fe_vec_for_each(skel_data->bones, bone_data)
    {
        fe_entity_id bone_id = fe_ecs_find_entity_id(skel->skeleton_id, bone_data->name);
        fe_entity_id parent_id = fe_ecs_find_entity_id(skel->skeleton_id, bone_data->parent_name);

        fe_entity_set_parent(bone_id, parent_id);
    }

	return rc;
}

void
fe_skeleton_kill(fe_skeleton_t *skel)
{
    if (fe_ecs_is_valid_entity(skel->skeleton_id)) 
    {
        fe_ecs_del_entity(skel->skeleton_id);
    }
}

void
fe_skeleton_play_animation(fe_skeleton_t *skel, const char *name, bool loop)
{	
	fe_skeleton_animation_data_t *anim = fe_skeleton_data_get_animation_data(skel->skel_data, name);
    if (!anim)
    {
        fe_log_warn("Animation '%s' not found", name);
        return;
    }

	reset_anim(skel);

    skel->anim.loop    = loop;
	skel->anim.playing = true;

	skel->anim.cur_anim_data = anim;
	skel->anim.duration      = anim->duration;
	skel->anim.tick_rate     = anim->ticks_per_sec;
}

void
fe_skeleton_stop_animation(fe_skeleton_t *skel)
{
    skel->anim.playing = false;
	skel->anim.cur_frame = 0;
	skel->anim.time		 = 0;
}

void
fe_skeleton_pause_animation(fe_skeleton_t *skel)
{
    skel->anim.playing = false;
}

