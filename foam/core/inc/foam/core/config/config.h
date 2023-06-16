#pragma once

#include <foam/core/ext/sugar.h>
#include <foam/core/ext/hashmap.h>
#include <foam/core/ext/vector.h>
#include <foam/core/ext/value.h>
#include <foam/core/ext/io.h>
#include <foam/core/return_code.h>
#include <foam/core/config/tokenizer.h>


#define FE_CFG_VAL_LIST_AT(v, index)      fe_value_get_list_at(v, index)
#define FE_CFG_VAL_AS_LIST(v)             fe_value_as_list(v)
#define FE_CFG_VAL_AS_STRING(v)           fe_value_as_string(v)
#define FE_CFG_VAL_AS_NUMBER(v)           fe_value_as_number(v)
#define FE_CFG_VAL_AS_TOKEN(v)            fe_value_as_token(v)
#define FE_CFG_VAL_IS_LIST(v)             fe_value_is_list(v)
#define FE_CFG_VAL_LIST_COUNT(v)          fe_value_get_list_count(v)
#define FE_CFG_PROP_AS_STRING(obj, name)  FE_CFG_VAL_AS_STRING(FE_CFG_VAL_LIST_AT(fe_config_object_get_value(obj, name), 0));
#define FE_CFG_PROP_AS_NUMBER(obj, name)  FE_CFG_VAL_AS_NUMBER(FE_CFG_VAL_LIST_AT(fe_config_object_get_value(obj, name), 0));

typedef struct
{
    char *name;
    fe_value_t value;
} fe_config_property_t;

typedef struct
{
    char                         *name;
    fe_vec(fe_config_property_t) properties;
    fe_hashmap_t                 property_lookup;
} fe_config_object_t;

typedef struct
{ 
    fe_vec(fe_config_object_t) objects;
    fe_hashmap_t               object_lookup;
} fe_config_t;


void                  fe_config_make(fe_config_t *cfg);
FE_RETURN_CODE        fe_config_make_from_file(fe_config_t *cfg, const char *path);
FE_RETURN_CODE        fe_config_make_from_memory(fe_config_t *cfg, const char *data, size_t data_len);
void                  fe_config_kill(fe_config_t *cfg);
void                  fe_config_save(fe_config_t *cfg, const char *path);
fe_config_object_t*   fe_config_add_object(fe_config_t *cfg, const char *name);
fe_config_object_t*   fe_config_dup_object(fe_config_t *cfg, const fe_config_object_t *src);
void                  fe_config_del_object(fe_config_t *cfg, const char *name);
void                  fe_config_del_object_at(fe_config_t *cfg, u64 index);
size_t                fe_config_get_object_count(const fe_config_t *cfg);
fe_config_object_t*   fe_config_get_object_at(const fe_config_t *cfg, u64 index);
fe_config_object_t*   fe_config_get_object(const fe_config_t *cfg, const char *obj_name);
size_t                fe_config_object_get_property_count(const fe_config_object_t *data);

fe_config_property_t* fe_config_object_get_property_at(const fe_config_object_t *data, size_t index);
fe_value_t*    fe_config_object_add_property(fe_config_object_t *obj, const char *prop_name);
void                  fe_config_object_del_property(fe_config_object_t *obj, const char *prop_name);
void                  fe_config_object_del_property_at(fe_config_object_t *obj, u64 index);
const char*           fe_config_property_get_name(const fe_config_property_t *prop);
fe_value_t*    fe_config_property_get_value(fe_config_property_t *prop);
fe_value_t*    fe_config_object_get_value(const fe_config_object_t *data, const char *property);
const char*           fe_config_object_get_name(const fe_config_object_t *data);
