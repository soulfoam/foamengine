#include <foam/core/ext/value.h>

void 
fe_value_make_string(fe_value_t *val, const char *str)
{
    val->str     = strdup(str);
    val->str_len = strlen(str);
    val->type    = FE_VALUE_STRING;
}

void 
fe_value_make_number(fe_value_t *val, f32 num)
{
    val->num  = num;
    val->type = FE_VALUE_NUMBER;
}

void 
fe_value_make_list(fe_value_t *val)
{
    val->list = FE_VEC_EMPTY;
    val->type = FE_VALUE_LIST;
}

void 
fe_value_add_value_to_list(fe_value_t *val, const fe_value_t *add)
{
    fe_value_t *v = NULL;
    fe_vec_add_and_get(val->list, v);
    *v = fe_value_dup(add);
}

void 
fe_value_add_number_to_list(fe_value_t *val, f32 number)
{
    fe_value_t *v = NULL;
    fe_vec_add_and_get(val->list, v);
    fe_value_make_number(v, number);
}

void 
fe_value_add_string_to_list(fe_value_t *val, const char *str)
{
    fe_value_t *v = NULL;
    fe_vec_add_and_get(val->list, v);
    fe_value_make_string(v, str);
}

void              
fe_value_add_vec2_to_list(fe_value_t *val, const fe_vec2_t *v)
{
    fe_value_t cv;
    fe_value_make_list(&cv);

    fe_value_add_number_to_list(&cv, v->x);
    fe_value_add_number_to_list(&cv, v->y);
    fe_value_add_value_to_list(val, &cv);

    fe_value_kill(&cv);
}

void              
fe_value_add_vec3_to_list(fe_value_t *val, const fe_vec3_t *v)
{
    fe_value_t cv;
    fe_value_make_list(&cv);

    fe_value_add_number_to_list(&cv, v->x);
    fe_value_add_number_to_list(&cv, v->y);
    fe_value_add_number_to_list(&cv, v->z);
    fe_value_add_value_to_list(val, &cv);

    fe_value_kill(&cv);
}

void              
fe_value_add_vec4_to_list(fe_value_t *val, const fe_vec4_t *v)
{
    fe_value_t cv;
    fe_value_make_list(&cv);

    fe_value_add_number_to_list(&cv, v->x);
    fe_value_add_number_to_list(&cv, v->y);
    fe_value_add_number_to_list(&cv, v->z);
    fe_value_add_number_to_list(&cv, v->w);
    fe_value_add_value_to_list(val, &cv);

    fe_value_kill(&cv);
}

void 
fe_value_kill(fe_value_t *cv)
{
    if (cv->type == FE_VALUE_STRING) free(cv->str);
    if (cv->type == FE_VALUE_LIST)   fe_vec_kill(cv->list, fe_value_kill);
}

fe_value_t 
fe_value_dup(const fe_value_t *cv)
{
    fe_value_t val;
    if      (cv->type == FE_VALUE_NUMBER) fe_value_make_number(&val, cv->num);
    else if (cv->type == FE_VALUE_STRING) fe_value_make_string(&val, cv->str);
    else if (cv->type == FE_VALUE_LIST)   
    {
        fe_value_make_list(&val);
        fe_vec_for_each(cv->list, v)
        {
            fe_value_t nv = fe_value_dup(v);
            fe_value_add_value_to_list(&val, &nv);
            fe_value_kill(&nv);
        }
    }
    else    fe_value_make_list(&val);

    return val;
}

void
fe_value_print(const fe_value_t *sv)
{ 
    if (sv->type == FE_VALUE_NUMBER)
    {
        printf("%f\n", sv->num);
    }
    else if (sv->type == FE_VALUE_STRING) 
    {
        printf("\"%s\"\n", sv->str);
    }
    else if (sv->type == FE_VALUE_LIST)
    {
        printf("(");
        fe_vec_for_each(sv->list, l)
        {
            fe_value_print(l);
        }
        printf(")\n");
    }
}

bool                  
fe_value_is_list(const fe_value_t *val)
{
    return val->type == FE_VALUE_LIST;
}

u64                   
fe_value_get_list_count(const fe_value_t *val)
{
    if (!val || val->type != FE_VALUE_LIST) return 0;
    return fe_vec_get_count(val->list);
}

fe_value_t* 
fe_value_get_list_at(const fe_value_t *val, u64 index)
{
    if (!val || val->type != FE_VALUE_LIST || index >= fe_vec_get_count(val->list)) return NULL;
    return &val->list[index];
}

const char*           
fe_value_as_string(const fe_value_t *val)
{
    if (!val || val->type != FE_VALUE_STRING) return NULL;
    return val->str;
}

f64
fe_value_as_number(const fe_value_t *val)
{
    if (!val || val->type != FE_VALUE_NUMBER) return 0;
    return val->num;
}

fe_vec2_t                 
fe_value_as_vec2(const fe_value_t *val)
{
    fe_vec2_t v = fe_vec2(0, 0);

    v.x = fe_value_as_number(fe_value_get_list_at(val, 0));
    v.y = fe_value_as_number(fe_value_get_list_at(val, 1));

    return v;
}

fe_vec3_t                 
fe_value_as_vec3(const fe_value_t *val)
{
    fe_vec3_t v = fe_vec3(0, 0, 0);

    v.x = fe_value_as_number(fe_value_get_list_at(val, 0));
    v.y = fe_value_as_number(fe_value_get_list_at(val, 1));
    v.z = fe_value_as_number(fe_value_get_list_at(val, 2));

    return v;
}

fe_vec4_t                 
fe_value_as_vec4(const fe_value_t *val)
{
    fe_vec4_t v = fe_vec4(0, 0, 0, 0);

    v.x = fe_value_as_number(fe_value_get_list_at(val, 0));
    v.y = fe_value_as_number(fe_value_get_list_at(val, 1));
    v.z = fe_value_as_number(fe_value_get_list_at(val, 2));
    v.w = fe_value_as_number(fe_value_get_list_at(val, 3));

    return v;
}
