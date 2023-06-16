#include <stdlib.h>
#include <ctype.h>
#include <foam/core/config/config.h>
#include <foam/core/ext/string.h>

#define write_data(writer, str, ...)  \
do \
{ \
    size_t needed   = snprintf(NULL, 0, str, __VA_ARGS__); \
    char *buffer    = malloc(needed+1); \
    sprintf(buffer, str, __VA_ARGS__); \
    fe_io_write_data(writer, buffer, needed); \
    free(buffer);  \
} while(0)

static void
kill_config_property(fe_config_property_t *prop)
{
    free(prop->name);
    fe_value_kill(&prop->value);
}

static void
kill_config_object(fe_config_object_t *cobj)
{
    free(cobj->name);
    fe_vec_kill(cobj->properties, kill_config_property);
    fe_hashmap_kill(&cobj->property_lookup);
}

static FE_RETURN_CODE
read_list(fe_value_t *dst, fe_token_list_t *tl, u64 *index)
{
    FE_RETURN_CODE rc = FE_OK;

    if (fe_token_list_get_at(tl, *index)->type != FE_TOKEN_LIST_OPEN)
    {
        fe_log_error("%s:%u - Missing list opening '('", tl->path, fe_token_list_get_at(tl, *index)->line_num); 
        return FE_ERR;
    }

    *index += 1;

    while (*index < fe_token_list_get_count(tl))
    {
        fe_token_t *tok = fe_token_list_get_at(tl, *index);

        if (tok->type == FE_TOKEN_STR)
        {
            fe_value_add_string_to_list(dst, tok->str_data);
        }
        else if (tok->type == FE_TOKEN_NUM)
        {
            fe_value_add_number_to_list(dst, tok->num_data);
        }
        else if (tok->type == FE_TOKEN_LIST_OPEN)
        {
            fe_value_t *sv = NULL;
            fe_vec_add_and_get(dst->list, sv);
            fe_value_make_list(sv);
            rc = read_list(sv, tl, index); 

            if (rc != FE_OK) return rc;

            continue;
        }
        else if (tok->type == FE_TOKEN_IDEN)
        {

        }
        else if (tok->type == FE_TOKEN_LIST_CLOSE)
        {
            *index += 1;
            break;
        }
        else
        {
            fe_log_error("%s:%u - Invalid value", tl->path, tok->line_num); 
            fe_token_print(tok);
            printf("\n");
            return FE_ERR;
        }

        *index += 1;
    }

    return rc;
}


static FE_RETURN_CODE
read_property(fe_config_property_t *dst, fe_token_list_t *tl, u64 *index)
{
    fe_token_t *prop_name = fe_token_list_get_at(tl, *index);
    if (prop_name->type != FE_TOKEN_IDEN)
    {
        fe_log_error("%s:%u - Property must be an identifier", tl->path, prop_name->line_num); 
        return FE_ERR;
    }

    *index += 1;

    dst->name = strdup(prop_name->str_data);
    fe_value_make_list(&dst->value);
    return read_list(&dst->value, tl, index);
}

static FE_RETURN_CODE
read_object(fe_config_object_t *dst, fe_token_list_t *tl, u64 *index)
{
    FE_RETURN_CODE rc = FE_OK;

    fe_token_t *name = fe_token_list_get_at(tl, *index);
    if (name->type != FE_TOKEN_IDEN)
    {
        fe_log_error("%s:%u - Object must be an identifier", tl->path, name->line_num); 
        return FE_ERR;
    }

    dst->name = strdup(name->str_data);

    *index += 1;

    if (fe_token_list_get_at(tl, *index)->type != FE_TOKEN_FUNC_OPEN)
    {
        fe_log_error("%s:%u - Missing object opening '{' for object '%s'", 
                tl->path, fe_token_list_get_at(tl, *index)->line_num, name->str_data); 
        return FE_ERR;
    }

    *index += 1;

    dst->properties = FE_VEC_EMPTY;
    fe_hashmap_make(&dst->property_lookup);

    while (*index < fe_token_list_get_count(tl))
    {
        fe_token_t *tok = fe_token_list_get_at(tl, *index);
        if (tok->type == FE_TOKEN_FUNC_CLOSE)
        {
            *index += 1;
            break;
        }

        fe_config_property_t *prop = NULL;
        fe_vec_add_and_get(dst->properties, prop);
        if (read_property(prop, tl, index) == FE_OK)
        {
            fe_hashmap_add(&dst->property_lookup, prop->name, fe_vec_get_last_index(dst->properties));
        }
        else
        {
            kill_config_object(dst);
            return rc; 
        }
    }

    return rc;
}

static void
write_value(fe_writer_t *writer, const fe_value_t *sv, bool new_line, bool eol_nl, bool whitespace)
{ 
    if (sv->type != FE_VALUE_LIST)
    {
        if (sv->type == FE_VALUE_STRING)
        {
            write_data(writer, "\"%s\"", fe_value_as_string(sv));
        }
        else if (sv->type == FE_VALUE_NUMBER)
        {
            write_data(writer, "%f", fe_value_as_number(sv));
        }

        if (whitespace) write_data(writer, "%s", " ");
    }
    else
    {
        if (new_line) write_data(writer, "%s", "\n\t");
        write_data(writer, "%s", "(");
        if (new_line) write_data(writer, "%s", "\n\t\t");

        fe_vec_for_each_index(sv->list, l, i)
        {
            write_value(writer, l, false, i != fe_value_get_list_count(sv)-1, i != fe_value_get_list_count(sv)-1);
        }

        if (new_line) write_data(writer, "%s", "\n\t");
        write_data(writer, "%s", ")");
        if (eol_nl) write_data(writer, "%s", "\n\t\t");
    }

}

static FE_RETURN_CODE
read_config(fe_config_t *cfg, fe_token_list_t *tl)
{
    FE_RETURN_CODE rc = FE_OK;

    u64 index = 0;
    while (index < fe_token_list_get_count(tl))
    {
        fe_config_object_t *cobj = NULL;
        fe_vec_add_and_get(cfg->objects, cobj);
        rc = read_object(cobj, tl, &index);
        fe_hashmap_add(&cfg->object_lookup, cobj->name, fe_vec_get_last_index(cfg->objects));
        if (rc != FE_OK) break;
    }

    return rc;
}

void
fe_config_make(fe_config_t *cfg)
{
    cfg->objects = FE_VEC_EMPTY;
    fe_hashmap_make(&cfg->object_lookup);
}

FE_RETURN_CODE
fe_config_make_from_memory(fe_config_t *cfg, const char *data, size_t data_len)
{
    fe_config_make(cfg);

    FE_RETURN_CODE rc = FE_OK;
    
    fe_token_list_t tl;
    rc = fe_token_list_make_from_memory(&tl, data, data_len);
    if (rc != FE_OK) return rc;

    rc = read_config(cfg, &tl);
    fe_token_list_kill(&tl);

    return rc;
}

FE_RETURN_CODE
fe_config_make_from_file(fe_config_t *cfg, const char *path)
{
    fe_config_make(cfg);

    FE_RETURN_CODE rc = FE_OK;
    
    fe_token_list_t tl;
    rc = fe_token_list_make_from_file(&tl, path);
    if (rc != FE_OK) 
    {
        fe_log_warn("Failed to load config file '%s'", path);
        return rc;
    }

    rc = read_config(cfg, &tl);
    fe_token_list_kill(&tl);

    return rc;
}

void
fe_config_kill(fe_config_t *cfg)
{
    fe_vec_kill(cfg->objects, kill_config_object);
    fe_hashmap_kill(&cfg->object_lookup);
}

void
fe_config_save(fe_config_t *cfg, const char *path)
{
    fe_writer_t writer;
    fe_writer_make(&writer);
    fe_vec_for_each(cfg->objects, obj)
    {
        write_data(&writer, "%s\n{\n", obj->name); 
        fe_vec_for_each_index(obj->properties, prop, p)
        {    
            write_data(&writer, "\t%s ", prop->name); 
            write_value(&writer, &prop->value, true, true, false);
            write_data(&writer, "%s", "\n"); 
        } 
        write_data(&writer, "%s", "\n}\n\n"); 
    }

    if (fe_writer_save(&writer, path, "wb"))
    {
        printf("\n--- %s (%lu bytes) successfully exported.\n\n", path, writer.size);
    }
    else
    {
        printf("\n--- %s (%lu bytes) FAILED to export!\n\n", path, writer.size);
    }

    fe_writer_kill(&writer);
}

fe_config_object_t*
fe_config_add_object(fe_config_t *cfg, const char *name)
{
    fe_config_object_t *obj = NULL;
    fe_vec_add_and_get(cfg->objects, obj);
    obj->name = strdup(name);
    obj->properties = FE_VEC_EMPTY;
    fe_hashmap_make(&obj->property_lookup);

    fe_hashmap_add(&cfg->object_lookup, name, fe_vec_get_last_index(cfg->objects));

    return obj;
}

fe_config_object_t*
fe_config_dup_object(fe_config_t *cfg, const fe_config_object_t *src)
{
    fe_config_object_t *obj = NULL;
    fe_vec_add_and_get(cfg->objects, obj);
    obj->name = strdup(src->name);
    obj->properties = FE_VEC_EMPTY;
    obj->property_lookup = fe_hashmap_dup(&src->property_lookup);
    fe_vec_for_each(src->properties, prop)
    {
        fe_value_t *val = fe_config_object_add_property(obj, prop->name);
        fe_value_kill(val); 
        *val = fe_value_dup(&prop->value);
    }

    return obj;
}

void
fe_config_del_object(fe_config_t *cfg, const char *name)
{
    u64 index = fe_hashmap_get(&cfg->object_lookup, name);
    if (index == FE_INVALID_INDEX) return;

    fe_hashmap_del(&cfg->object_lookup, name);
    fe_vec_del(cfg->objects, index, kill_config_object);
}

void
fe_config_del_object_at(fe_config_t *cfg, u64 index)
{
    fe_hashmap_del(&cfg->object_lookup, cfg->objects[index].name);
    fe_vec_del(cfg->objects, index, kill_config_object);
}

size_t
fe_config_get_object_count(const fe_config_t *cfg)
{
    return fe_vec_get_count(cfg->objects);
}

fe_config_object_t*
fe_config_get_object_at(const fe_config_t *cfg, u64 index)
{
    return fe_vec_get(cfg->objects, index);
}

fe_config_object_t* 
fe_config_get_object(const fe_config_t *cfg, const char *obj_name)
{ 
    u64 index = fe_hashmap_get(&cfg->object_lookup, obj_name);
    if (index == FE_INVALID_INDEX) return NULL;

    return &cfg->objects[index];
}

const char*
fe_config_object_get_name(const fe_config_object_t *cobj)
{
    return cobj->name;
}

fe_value_t* 
fe_config_object_get_value(const fe_config_object_t *cobj, const char *property)
{ 
    u64 index = fe_hashmap_get(&cobj->property_lookup, property);
    if (index == FE_INVALID_INDEX) return NULL;

    return &cobj->properties[index].value;
}

size_t                   
fe_config_object_get_property_count(const fe_config_object_t *obj)
{
    return fe_vec_get_count(obj->properties);
}

fe_config_property_t*  
fe_config_object_get_property_at(const fe_config_object_t *obj, size_t index)
{
    return &obj->properties[index];
}

fe_value_t*
fe_config_object_add_property(fe_config_object_t *obj, const char *prop_name)
{ 
    fe_config_property_t *prop = NULL;
    fe_vec_add_and_get(obj->properties, prop);
    prop->name  = strdup(prop_name);
    fe_value_make_list(&prop->value);
    fe_hashmap_add(&obj->property_lookup, prop->name, fe_vec_get_last_index(obj->properties));

    return &prop->value;
}

void                  
fe_config_object_del_property(fe_config_object_t *obj, const char *prop_name)
{
    u64 index = fe_hashmap_get(&obj->property_lookup, prop_name);
    if (index != FE_INVALID_INDEX)
    {
        fe_hashmap_del(&obj->property_lookup, prop_name);
        fe_vec_del(obj->properties, index, kill_config_property);
    }
}

void                  
fe_config_object_del_property_at(fe_config_object_t *obj, u64 index)
{
    fe_vec_del(obj->properties, index, kill_config_property);
}

const char*
fe_config_property_get_name(const fe_config_property_t *prop)
{
    return prop->name;
}

fe_value_t*
fe_config_property_get_value(fe_config_property_t *prop)
{
    return &prop->value;
}
