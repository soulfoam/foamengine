#pragma once

#include <foam/core/math/math.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/return_code.h>
#include <foam/core/ext/vector.h>

FE_DEFINE_HANDLE(fe_skeleton_data_id);

#define FE_SKELETON_DATA_VERSION 1

typedef struct
{
	char *name;

	fe_vec(fe_vec3_t) translations;
	fe_vec(fe_quat_t) rotations;
	fe_vec(fe_vec3_t) scales;	

} fe_bone_animation_data_t;

typedef struct
{
	char *name;
	f32 duration;
	f32 ticks_per_sec;
	fe_vec(fe_bone_animation_data_t) bone_anims;

} fe_skeleton_animation_data_t;

typedef struct
{	
	char *name;	
	char *parent_name;	

	fe_mat4x4_t offset_matrix;

} fe_bone_data_t;

typedef struct
{
    fe_vec(fe_bone_data_t)               bones;
	fe_vec(fe_skeleton_animation_data_t) anims;

} fe_skeleton_data_t;

void                          fe_skeleton_data_make(fe_skeleton_data_t *skel_data);
FE_RETURN_CODE                fe_skeleton_data_make_from_file(fe_skeleton_data_t *skel_data, const char *path);
void                          fe_skeleton_data_kill(fe_skeleton_data_t *skel_data);
fe_bone_data_t*               fe_skeleton_data_get_bone_data(const fe_skeleton_data_t *skel_data, const char *bone_str);
fe_skeleton_animation_data_t* fe_skeleton_data_get_animation_data(const fe_skeleton_data_t *skel_data, const char *anim_str);
fe_bone_animation_data_t*     fe_skeleton_animation_data_get_bone_animation_data(const fe_skeleton_animation_data_t *anim, const char *name);

