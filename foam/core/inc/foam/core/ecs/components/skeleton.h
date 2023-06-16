#pragma once

#include <foam/core/ecs/ecs.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/vector.h>
#include <foam/core/data/skeleton_data.h>

typedef struct
{
	bool playing;
	bool played_once;
	bool loop;
	f32  time;
	u32  cur_frame;
	u32  next_frame;
	f32  duration;
	f32  tick_rate;

	const fe_skeleton_animation_data_t *cur_anim_data;
} fe_skeleton_animation_t;

typedef struct
{
    fe_entity_id skeleton_id;

	const fe_skeleton_data_t *skel_data;
	fe_skeleton_animation_t anim;
	
} fe_skeleton_t;

void           fe_skeleton_make(fe_skeleton_t *skel, const fe_skeleton_data_t *skel_data);
FE_RETURN_CODE fe_skeleton_make_with_bones(fe_skeleton_t *skel, const fe_skeleton_data_t *skel_data, fe_entity_id parent_id, const char *skeleton_parent_name);
void           fe_skeleton_kill(fe_skeleton_t *skel);
void           fe_skeleton_play_animation(fe_skeleton_t *skel, const char *name, bool loop);
void           fe_skeleton_stop_animation(fe_skeleton_t *skel);
void           fe_skeleton_pause_animation(fe_skeleton_t *skel);

