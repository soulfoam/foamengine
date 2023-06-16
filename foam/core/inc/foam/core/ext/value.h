#pragma once

#include <foam/core/ext/sugar.h>
#include <foam/core/ext/vector.h>
#include <foam/core/math/math.h>

typedef enum
{
    FE_VALUE_STRING,
    FE_VALUE_NUMBER,
    FE_VALUE_LIST,

} FE_VALUE_TYPE;

typedef struct
fe_value_t
{
    union
    {
        struct
        {        
            char *str;
            size_t str_len;
        };

        f32 num;
        fe_vec(struct fe_value_t) list;
    };

    u8 type;

} fe_value_t;


void        fe_value_make_string(fe_value_t *val, const char *str);
void        fe_value_make_number(fe_value_t *val, f32 num);
void        fe_value_make_list(fe_value_t *val);
void        fe_value_kill(fe_value_t *val);
fe_value_t  fe_value_dup(const fe_value_t *cv);
void        fe_value_add_value_to_list(fe_value_t *val, const fe_value_t *add);
void        fe_value_add_number_to_list(fe_value_t *val, f32 number);
void        fe_value_add_vec2_to_list(fe_value_t *val, const fe_vec2_t *v);
void        fe_value_add_vec3_to_list(fe_value_t *val, const fe_vec3_t *v);
void        fe_value_add_vec4_to_list(fe_value_t *val, const fe_vec4_t *v);
void        fe_value_add_string_to_list(fe_value_t *val, const char *str);
bool        fe_value_is_list(const fe_value_t *val);
u64         fe_value_get_list_count(const fe_value_t *val);
fe_value_t* fe_value_get_list_at(const fe_value_t *val, u64 index);
const char* fe_value_as_string(const fe_value_t *val);
f64         fe_value_as_number(const fe_value_t *val);
fe_vec2_t   fe_value_as_vec2(const fe_value_t *val);
fe_vec3_t   fe_value_as_vec3(const fe_value_t *val);
fe_vec4_t   fe_value_as_vec4(const fe_value_t *val);
void        fe_value_print(const fe_value_t *val);
