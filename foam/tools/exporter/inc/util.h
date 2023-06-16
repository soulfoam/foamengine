#pragma once

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/math/math.h>
#include <foam/core/ext/io.h>
#include <foam/core/config/config.h>

#define write_data(writer, str, ...)  \
do \
{ \
    size_t needed   = snprintf(NULL, 0, str, __VA_ARGS__); \
    char *buffer    = malloc(needed+1); \
    sprintf(buffer, str, __VA_ARGS__); \
    fe_io_write_data(writer, buffer, needed); \
    free(buffer);  \
} while(0)

static inline void
write_header(fe_writer_t *writer)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%c", tm);
    write_data(writer, "# --- fe_exporter [%s] --- #\n", time_str);
}

typedef enum
{
	OBJ,
	FBX,
	DAE
} FileType;


static inline FileType
get_file_type(const char *input_path)
{
	if (strstr(input_path, ".obj") != NULL) return OBJ;
	if (strstr(input_path, ".fbx") != NULL) return FBX;
	if (strstr(input_path, ".dae") != NULL) return DAE;

    return 0;
}

static inline bool
f_eq(float a, float b)
{
	float dec = 1000000.0f;
	a = floor(dec*a)/dec;
	b = floor(dec*b)/dec;

	return a == b;
}

static inline fe_mat4x4_t
aiMat4_to_mat4(const struct aiMatrix4x4 *from)
{	
    fe_mat4x4_t r;

	r.raw[0][0] = from->a1; r.raw[1][0] = from->a2;
	r.raw[2][0] = from->a3; r.raw[3][0] = from->a4;
	r.raw[0][1] = from->b1; r.raw[1][1] = from->b2;
	r.raw[2][1] = from->b3; r.raw[3][1] = from->b4;
	r.raw[0][2] = from->c1; r.raw[1][2] = from->c2;
	r.raw[2][2] = from->c3; r.raw[3][2] = from->c4;
	r.raw[0][3] = from->d1; r.raw[1][3] = from->d2;
	r.raw[2][3] = from->d3; r.raw[3][3] = from->d4;

    return r;
}

static inline void
aiMat4_decompose(struct aiMatrix4x4 *mtx, fe_vec3_t *trl, fe_quat_t *quat, fe_vec3_t *scl)
{	
    struct aiVector3D   atrl;
    struct aiQuaternion arot;
    struct aiVector3D   ascl;
    aiDecomposeMatrix(mtx, &ascl, &arot, &atrl);

    trl->x = atrl.x;
    trl->y = atrl.y;
    trl->z = atrl.z;

    quat->x = arot.x;
    quat->y = arot.y;
    quat->z = arot.z;
    quat->w = arot.w;

    scl->x = ascl.x;
    scl->y = ascl.y;
    scl->z = ascl.z;
}

static inline fe_config_object_t*
config_get_object_with_property_name(fe_config_t *cfg, const char *name)
{
    fe_vec_for_each(cfg->objects, obj) 
    {            
        const char *val = FE_CFG_PROP_AS_STRING(obj, "name");
        if (!val) continue;
        if (STR_EQ(val, name)) return obj;
    }
    
    return NULL;
}

static inline void
config_update_entity_object(fe_config_object_t *dst, fe_config_object_t *other)
{   
    fe_vec_for_each(dst->properties, prop)
    {
        fe_value_t *val = fe_config_object_get_value(other, prop->name);
        if (!val) continue;

        fe_value_kill(&prop->value);
        prop->value = fe_value_dup(val);
    }
}

static inline void
config_update_entities(fe_config_t *prev_cfg, fe_config_t *cfg)
{
    fe_vec_for_each(prev_cfg->objects, obj) 
    {             
        const char *name = FE_CFG_PROP_AS_STRING(obj, "name");
        if (!name) continue;
        fe_config_object_t *other = config_get_object_with_property_name(cfg, name);
        if (!other) continue;

        config_update_entity_object(obj, other);
    }
}

static inline void
config_add_missing_entities(fe_config_t *prev_cfg, fe_config_t *cfg)
{
    fe_vec_for_each(cfg->objects, obj) 
    {             
        const char *name = FE_CFG_PROP_AS_STRING(obj, "name");
        if (!name) continue;
        fe_config_object_t *other = config_get_object_with_property_name(prev_cfg, name);
        if (!other) 
        {
            fe_config_dup_object(prev_cfg, obj);
        }

    }
}
