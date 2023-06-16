#include <string.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/logger.h>
#include <foam/core/ext/io.h>
#include <foam/core/data/skeleton_data.h>

void
fe_skeleton_data_make(fe_skeleton_data_t *skel_data)
{
    skel_data->anims = NULL;
    skel_data->bones = NULL;
}

FE_RETURN_CODE
fe_skeleton_data_make_from_file(fe_skeleton_data_t *skel_data, const char *path)
{
    FE_RETURN_CODE rc = FE_OK;

    fe_reader_t r;
    rc = fe_reader_make_from_file(&r, path);
    if (rc != FE_OK)
	{
        fe_log_return_code(rc, "Skeleton Data [%s] could not be initialized", path);
        return rc;
	}

    u32 version = fe_io_read_u32(&r);
	if (version != FE_SKELETON_DATA_VERSION)
	{
        rc = FE_ERR_DATA_VERSION_MISMATCH;
        fe_log_return_code(rc, "Skeleton Data [%s] version mismatch. Skeleton Data version is %u and Engine version is %u", path, version, FE_SKELETON_DATA_VERSION);
        fe_reader_kill(&r);
        return rc;
	} 
	
    u32 bone_count  = fe_io_read_u32(&r);
    u32 anim_count  = fe_io_read_u32(&r);
    fe_vec_make_with_count(skel_data->bones, bone_count);
    fe_vec_make_with_count(skel_data->anims, anim_count);

    for (u32 i = 0; i < bone_count; i++)
    {
        fe_bone_data_t *bone_data = &skel_data->bones[i];

        bone_data->name        = fe_io_read_string(&r);
        bone_data->parent_name = fe_io_read_string(&r);

        bone_data->offset_matrix = fe_io_read_mat4x4(&r);
    }

	for (u32 i = 0; i < anim_count; i++)
	{
        fe_skeleton_animation_data_t *anim_data = &skel_data->anims[i];

        anim_data->name          = fe_io_read_string(&r);
        anim_data->duration      = fe_io_read_f32(&r);
        anim_data->ticks_per_sec = fe_io_read_f32(&r);
        u32 bone_anim_count      = fe_io_read_f32(&r);

        fe_vec_make_with_count(anim_data->bone_anims, bone_anim_count);

        for (u32 j = 0; j < bone_anim_count; j++)
        {
            fe_bone_animation_data_t *bone_anim = &anim_data->bone_anims[j];

            bone_anim->name       = fe_io_read_string(&r);
            u32 translation_count = fe_io_read_u32(&r);
            u32 rotation_count    = fe_io_read_u32(&r);
            u32 scale_count       = fe_io_read_u32(&r);

            fe_vec_make_with_count(bone_anim->translations, translation_count);
            fe_vec_make_with_count(bone_anim->rotations, rotation_count);
            fe_vec_make_with_count(bone_anim->scales, scale_count);

            for (u32 c = 0; c < translation_count; c++)
            {
                bone_anim->translations[c] = fe_io_read_vec3(&r);
            }
            for (u32 c = 0; c < rotation_count; c++)
            {
                bone_anim->rotations[c] = fe_io_read_quat(&r);
            }
            for (u32 c = 0; c < scale_count; c++)
            {
                bone_anim->scales[c] = fe_io_read_vec3(&r);
            }
        }
    }

    fe_reader_kill(&r);

    return rc;
}

static void
bone_animation_data_kill(fe_bone_animation_data_t *ba)
{ 
    free(ba->name);
    fe_vec_kill(ba->translations, NULL);
    fe_vec_kill(ba->rotations,    NULL);
    fe_vec_kill(ba->scales,       NULL);
}

static void
skeleton_animation_data_kill(fe_skeleton_animation_data_t *sa)
{
    free(sa->name);
    fe_vec_kill(sa->bone_anims, bone_animation_data_kill);
}

static void
bone_data_kill(fe_bone_data_t *bd)
{
    free(bd->name);
    free(bd->parent_name);
}

void
fe_skeleton_data_kill(fe_skeleton_data_t *data)
{
    fe_vec_kill(data->bones, bone_data_kill);
    fe_vec_kill(data->anims, skeleton_animation_data_kill); 
}

fe_bone_data_t*
fe_skeleton_data_get_bone_data(const fe_skeleton_data_t *skel, const char *bone_str)
{
    fe_vec_for_each(skel->bones, bone)
	{
		if (STR_EQ(bone->name, bone_str)) return bone;
	}

	return NULL;
}

fe_skeleton_animation_data_t*
fe_skeleton_data_get_animation_data(const fe_skeleton_data_t *skel_data, const char *anim_str)
{
    fe_vec_for_each(skel_data->anims, anim)
	{
		if (STR_EQ(anim->name, anim_str)) return anim;
	}

	return NULL;
}

fe_bone_animation_data_t*
fe_skeleton_animation_data_get_bone_animation_data(const fe_skeleton_animation_data_t *anim, const char *name)
{
    fe_vec_for_each(anim->bone_anims, a)
	{
		if (STR_EQ(a->name, name)) return a;
	}

	return NULL;
}

