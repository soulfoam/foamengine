#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/vector.h>
#include <foam/core/ext/logger.h>
#include <foam/core/config/config.h>
#include <foam/core/data/skeleton_data.h>
#include <foam/core/ext/string.h>
#include <float.h>
#include <assert.h>
#include <export_skeleton.h>


/*
 * NOTES:
 *
 * 1. NO SUPPORT FOR LEAF BONES IN FBX
 * 2. IN BLENDER, YOU MUST GO INTO EDIT MODE AND ASSIGN VERTEX GROUPS THERE.
 * SELECT MESH THEN BONE AND DO CTRL + P AND SELECT EMPTY GROUPS 
 * 3. IN BLENDER SIMPLIFY MUST BE DISABLED (set to 0.0) so that every bone has the same
 * amount of keyframes (which is just a key frame for the entire duration)
 * 
 */


static const struct aiScene *scene = NULL;
struct aiNode *root_bone_node      = NULL;
static u32 input_file_type         = FBX;

static fe_vec(fe_bone_data_t) bones               = FE_VEC_EMPTY;
static fe_vec(fe_skeleton_animation_data_t) anims = FE_VEC_EMPTY;

static void
cleanup(void)
{	
    FE_TODO("WRITE PROPER CLEANUP");
    fe_vec_kill(bones, NULL);
    fe_vec_kill(anims, NULL);
	aiReleaseImport(scene);
}

static struct aiNode*
get_node(struct aiNode *node, const char *name)
{
	if (STR_EQ(name, node->mName.data)) return node;
	for (u32 i = 0; i < node->mNumChildren; i++)
	{
		struct aiNode *n = get_node(node->mChildren[i], name);
		if (n) return n;
	}

	return NULL;
}

static struct aiBone*
get_bone(const char *name)
{
	for (u32 i = 0; i < scene->mNumMeshes; i++)
	{
		struct aiMesh *m = scene->mMeshes[i];
		for (u32 j = 0; j < m->mNumBones; j++)
		{
			struct aiBone *b = m->mBones[j];
			if (STR_EQ(name, b->mName.data)) return b;
		}
	}

	return NULL;
}

static const char*
get_bone_mesh_name(const char *bone_name)
{
	for (u32 i = 0; i < scene->mNumMeshes; i++)
	{
		struct aiMesh *m = scene->mMeshes[i];
		for (u32 j = 0; j < m->mNumBones; j++)
		{
			struct aiBone *b = m->mBones[j];
            if (STR_EQ(b->mName.data, bone_name) && b->mNumWeights > 0)
            {
                return m->mName.data; 
            }
		}
	}

	return NULL;
}

static struct aiNode*
get_root_bone(void)
{
	for (u32 i = 0; i < scene->mNumMeshes; i++)
	{
		struct aiMesh *m = scene->mMeshes[i];
		for (u32 j = 0; j < m->mNumBones; j++)
		{
			struct aiBone *b           = m->mBones[j];
			struct aiNode *node        = get_node(scene->mRootNode, b->mName.data);
			struct aiBone *parent_bone = get_bone(node->mParent->mName.data);
			if (!parent_bone) return node;
		}
	}
	
    fe_log("No root bone was found!");

	return NULL;
}

static void
build_bone_tree(struct aiNode *bone_node)
{	

	fe_bone_data_t *b = NULL;
    fe_vec_add_and_get(bones, b);

    b->name        = strdup(bone_node->mName.data);
    b->parent_name = strdup(bone_node->mParent->mName.data);

	struct aiBone *ai_bone = get_bone(bone_node->mName.data);
	
	if (!ai_bone)
	{
		fe_log_warn("[%s] was unable to be found on any of the meshes. Make sure the bone has vertices attached to it or else the bone won't be imported into the list of meshes correctly.\n", 
				bone_node->mName.data);
		cleanup();
		exit(1);
	}

	b->offset_matrix = aiMat4_to_mat4(&ai_bone->mOffsetMatrix);

	for (u32 i = 0; i < bone_node->mNumChildren; i++)
	{	
		struct aiNode *c = bone_node->mChildren[i];
		
		build_bone_tree(c);
	}
}

static bool
export_skeleton(const char *path)
{

	fe_skeleton_data_t skel_data;
	skel_data.path  = NULL;
	skel_data.bones = bones;
	skel_data.anims = anims;

    fe_writer_t writer;
    fe_writer_make(&writer);
    
    fe_io_write_u32(&writer, FE_SKELETON_DATA_VERSION);
    fe_io_write_u32(&writer, fe_vec_get_count(skel_data.bones));
    fe_io_write_u32(&writer, fe_vec_get_count(skel_data.anims));

    fe_vec_for_each(skel_data.bones, bone_data)
	{
        fe_io_write_string(&writer, bone_data->name);
        fe_io_write_string(&writer, bone_data->parent_name);
        fe_io_write_mat4x4(&writer, &bone_data->offset_matrix);
	}

    fe_vec_for_each(skel_data.anims, anim_data)
	{
        fe_io_write_string(&writer, anim_data->name);
        fe_io_write_f32(&writer, anim_data->duration);
        fe_io_write_f32(&writer, anim_data->ticks_per_sec);
        fe_io_write_f32(&writer, fe_vec_get_count(anim_data->bone_anims));    

        fe_vec_for_each(anim_data->bone_anims, bone_anim)
        {
            fe_io_write_string(&writer, bone_anim->name);
            fe_io_write_u32(&writer, fe_vec_get_count(bone_anim->translations));
            fe_io_write_u32(&writer, fe_vec_get_count(bone_anim->rotations));
            fe_io_write_u32(&writer, fe_vec_get_count(bone_anim->scales));

            fe_vec_for_each(bone_anim->translations, trl)
            { 
                fe_io_write_vec3(&writer, trl);
            }
            fe_vec_for_each(bone_anim->rotations, rot)
            { 
                fe_io_write_quat(&writer, rot);
            }
            fe_vec_for_each(bone_anim->scales, scl)
            { 
                fe_io_write_vec3(&writer, scl);
            }
        }
	}

    bool success = true;
	if (fe_writer_save(&writer, path, "wb"))
	{
		printf("\n--- %s (%lu bytes) successfully exported.\n\n", path, writer.size);
	}
	else
	{
		printf("\n--- %s (%lu bytes) FAILED to export!\n\n", path, writer.size);
        success = false;
	}
	
	fe_writer_kill(&writer);

    return success;
}

static void
config_add_bone_entities(fe_config_t *cfg)
{
    fe_vec_for_each(bones, bone)
    {
        fe_config_object_t *obj  = fe_config_add_object(cfg, "ENTITY");
        fe_value_t *pname  = fe_config_object_add_property(obj, "name");
        fe_value_add_string_to_list(pname, bone->name); 
        fe_value_t *pparent_name  = fe_config_object_add_property(obj, "parent");
        if (bone->parent_name)
        {
            fe_value_add_string_to_list(pparent_name, bone->parent_name); 
        }

        fe_value_t *pxform  = fe_config_object_add_property(obj, "transform");
        fe_value_add_vec3_to_list(pxform, &FE_VEC3_ZERO);
        fe_value_add_vec3_to_list(pxform, &FE_VEC3_ZERO);
        fe_value_add_vec3_to_list(pxform, &FE_VEC3_ONE);

        fe_value_t *pbone = fe_config_object_add_property(obj, "bone");

    }
}

static void
config_add_skeleton_entity(fe_config_t *cfg, const char *skeleton_data_path)
{
    fe_config_object_t *obj   = fe_config_add_object(cfg, "ENTITY");
    fe_value_t *pname  = fe_config_object_add_property(obj, "name");
    fe_value_add_string_to_list(pname, root_bone_node->mParent->mName.data);
    fe_value_t *pxform  = fe_config_object_add_property(obj, "transform");
    fe_value_add_vec3_to_list(pxform, &FE_VEC3_ZERO);
    fe_value_add_vec3_to_list(pxform, &FE_VEC3_ZERO);
    fe_value_add_vec3_to_list(pxform, &FE_VEC3_ONE);

    fe_value_t *pskeleton  = fe_config_object_add_property(obj, "skeleton");
    fe_value_add_string_to_list(pskeleton, skeleton_data_path);
}

static void
config_update_mesh_instance_parent_names(fe_config_t *cfg)
{
    fe_vec_for_each(bones, b)
    {
        const char *mesh_name = get_bone_mesh_name(b->name);
        if (!mesh_name) continue;

        fe_config_object_t *obj = config_get_object_with_property_name(cfg, mesh_name); 
        if (!obj) continue;

        fe_value_t *parent_name = fe_config_object_get_value(obj, "parent");


        if (parent_name)
        {
            fe_value_kill(parent_name);
        }
        else
        {
            parent_name = fe_config_object_add_property(obj, "parent");
        }

        fe_value_add_string_to_list(parent_name, b->name);
    }
}

static void
export_config(const char *skeleton_data_path, const char *config_path)
{
    fe_skeleton_data_t skel_data;
    if (fe_skeleton_data_make_from_file(&skel_data, skeleton_data_path) != FE_OK) return;

    fe_config_t cfg;
    fe_config_make(&cfg);

    config_add_skeleton_entity(&cfg, skeleton_data_path);
    config_add_bone_entities(&cfg);

    fe_config_t prev_cfg;
    if (fe_config_make_from_file(&prev_cfg, config_path) == FE_OK)
    {
        config_update_entities(&prev_cfg, &cfg);
        config_add_missing_entities(&prev_cfg, &cfg);
        config_update_mesh_instance_parent_names(&prev_cfg); 

        fe_config_save(&prev_cfg, config_path);
    }
    else
    {
        fe_config_save(&cfg, config_path);
    }

    fe_config_kill(&cfg);

}

void
fe_export_skeleton(const char *input_path, const char *output_path, const char *config_path)
{

    FE_TODO("We should at some point support the ability to have a variable number of keyframes for each bone. Currently every bone is forced to have the same number of translations, rotations and scales.");

	input_file_type = get_file_type(input_path);
	
	scene = aiImportFile(
			input_path, 
			aiProcess_Triangulate | aiProcess_GenSmoothNormals);

	if (!scene)
	{
		fe_log_error("Failed to load file %s\n%s\n", input_path, aiGetErrorString());
		return;
	}

	root_bone_node = get_root_bone();
    if (!root_bone_node) return;

	build_bone_tree(root_bone_node);

	for (u32 i = 0; i < scene->mNumAnimations; i++)
	{
		struct aiAnimation *a = scene->mAnimations[i];
	    	
		fe_skeleton_animation_data_t *anim = NULL;
        fe_vec_add_and_get(anims, anim);
		anim->duration      = a->mDuration;
		anim->ticks_per_sec = a->mTicksPerSecond;

		if (anim->ticks_per_sec <= 0) anim->ticks_per_sec = 24;

		//correct names by stripping
		if (input_file_type == FBX)
		{
			char *n_name = strchr(a->mName.data, '|')+1;

			if (n_name) 
            {
                anim->name = strdup(n_name);
            }
		}
		if (input_file_type == DAE)
		{
			//char *n_name = strchr(a->mName.data, '_')+1;
			//if (n_name) 
            //{
            //    fe_log("ok before %s", n_name);
            //    
            //    strncpy(anim->name, n_name, MAX_NAME_LEN);
            //    fe_log("ok after");
            //}
            //else
            //{ 
                anim->name = strdup(a->mName.data);
            //}
		}

		u32 bone_anim_count = a->mNumChannels-1;
        fe_vec_make_with_count(anim->bone_anims, bone_anim_count);

		//j = 1 because armature is the root animation, dont think we want it?
		for (u32 j = 1; j < a->mNumChannels; j++)
		{
			struct aiNodeAnim *node_anim = a->mChannels[j];
			fe_bone_animation_data_t *bone_anim = &anim->bone_anims[j-1];

            bone_anim->name = strdup(node_anim->mNodeName.data);

            fe_vec_make_with_count(bone_anim->translations, node_anim->mNumPositionKeys);
            fe_vec_make_with_count(bone_anim->rotations,    node_anim->mNumRotationKeys);
            fe_vec_make_with_count(bone_anim->scales,       node_anim->mNumScalingKeys);

			for (u32 p = 0; p < node_anim->mNumPositionKeys; p++)
			{
				bone_anim->translations[p].x = node_anim->mPositionKeys[p].mValue.x;
				bone_anim->translations[p].y = node_anim->mPositionKeys[p].mValue.y;
				bone_anim->translations[p].z = node_anim->mPositionKeys[p].mValue.z;
			}

			for (u32 r = 0; r < node_anim->mNumRotationKeys; r++)
			{
				bone_anim->rotations[r].x = node_anim->mRotationKeys[r].mValue.x;
				bone_anim->rotations[r].y = node_anim->mRotationKeys[r].mValue.y;
				bone_anim->rotations[r].z = node_anim->mRotationKeys[r].mValue.z;
				bone_anim->rotations[r].w = node_anim->mRotationKeys[r].mValue.w;
				
			}

			for (u32 s = 0; s < node_anim->mNumScalingKeys; s++)
			{
				bone_anim->scales[s].x = node_anim->mScalingKeys[s].mValue.x;
				bone_anim->scales[s].y = node_anim->mScalingKeys[s].mValue.y;
				bone_anim->scales[s].z = node_anim->mScalingKeys[s].mValue.z;
			}
		}
	}
	

	fe_log("\nBone List (%u):\n", fe_vec_get_count(bones));

    fe_vec_for_each(bones, b)
	{
	    printf("%s (parent: %s)\n", b->name, b->parent_name);
		//mat4_print(bones[i].offset_matrix);
	}
	
	fe_log("\nAnimation List (%u):\n", fe_vec_get_count(anims));
    fe_vec_for_each(anims, a)
	{
		printf("%s (bones %lu) (duration: %.1f) (ticks_per_sec: %.1f)\n", 
				a->name, fe_vec_get_count(a->bone_anims),
				a->duration, a->ticks_per_sec);

        u32 last_bone_trl_count = fe_vec_get_count(a->bone_anims[0].translations);
        u32 last_bone_rot_count = fe_vec_get_count(a->bone_anims[0].rotations);
        u32 last_bone_scl_count = fe_vec_get_count(a->bone_anims[0].scales);
        fe_vec_for_each(a->bone_anims, bone_anim)
		{
            u32 trl_count = fe_vec_get_count(bone_anim->translations);
            u32 rot_count = fe_vec_get_count(bone_anim->rotations);
            u32 scl_count = fe_vec_get_count(bone_anim->scales);
            assert(last_bone_trl_count == trl_count);
            assert(last_bone_rot_count == rot_count);
            assert(last_bone_scl_count == scl_count);
            last_bone_trl_count = trl_count;
            last_bone_rot_count = rot_count;
            last_bone_scl_count = scl_count;
			printf("---- %s ----\n", bone_anim->name);

			u32 keyframe_count = fe_vec_get_count(bone_anim->translations);
			for (u32 k = 0; k < keyframe_count; k++)
			{
				fe_vec3_t t = bone_anim->translations[k];
				fe_quat_t r = bone_anim->rotations[k];
				fe_vec3_t s = bone_anim->scales[k];

				printf("(%u) Translation: %f %f %f\n"
					   "(%u) Rotation: %f %f %f %f\n"
					   "(%u) Scale: %f %f %f\n",
						k, t.x, t.y, t.z, k, r.x, r.y, r.z, r.w, k, s.x, s.y, s.z);
			}
		}
	}

	bool exported = export_skeleton(output_path);

    if (exported && config_path)
    {
        export_config(output_path, config_path);
    }


	cleanup();

}


