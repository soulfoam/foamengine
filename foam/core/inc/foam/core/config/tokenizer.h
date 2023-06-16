#pragma once

#include <foam/core/return_code.h>
#include <foam/core/ext/vector.h>
#include <foam/core/ext/sugar.h>

typedef enum
{
    FE_TOKEN_INVALID,

    FE_TOKEN_IDEN,
    FE_TOKEN_NUM,
    FE_TOKEN_STR,

    FE_TOKEN_ARG,
    FE_TOKEN_DEFINE,

    FE_TOKEN_FUNC_OPEN,
    FE_TOKEN_FUNC_CLOSE,

    FE_TOKEN_LIST_OPEN,
    FE_TOKEN_LIST_CLOSE,


} FE_TOKEN_TYPE;

typedef struct
{
    FE_TOKEN_TYPE type;

    char   *str_data;
    size_t str_data_len;

    f64    num_data;

    size_t line_num;
    size_t col;
} fe_token_t;

typedef struct
{
    char *path;
    fe_vec(char*)      lines;
    fe_vec(fe_token_t) tokens;
} fe_token_list_t;

void           fe_token_list_make(fe_token_list_t *tl);
FE_RETURN_CODE fe_token_list_make_from_file(fe_token_list_t *tl, const char *path);
FE_RETURN_CODE fe_token_list_make_from_memory(fe_token_list_t *tl, const char *data, size_t data_len);
void           fe_token_list_make_token(fe_token_list_t *tl, FE_TOKEN_TYPE type, char *data, size_t data_len, size_t line, size_t col);
void           fe_token_list_kill(fe_token_list_t *tl);
void           fe_token_list_push_token(fe_token_list_t *tl, const fe_token_t *tok);
void           fe_token_list_push_number(fe_token_list_t *tl, const char *num);
void           fe_token_list_push_string(fe_token_list_t *tl, const char *str);
u32            fe_token_list_get_count(const fe_token_list_t *tl);
fe_token_t*    fe_token_list_get_at(const fe_token_list_t *tl, u32 index);
void           fe_token_list_print(const fe_token_list_t *tl);
void           fe_token_copy(fe_token_t *dst, const fe_token_t *src);
bool           fe_token_is_equal(const fe_token_t *tok, const fe_token_t *other);
void           fe_token_print(const fe_token_t *tok);
const char*    fe_token_get_type_str(FE_TOKEN_TYPE type);
const char*    fe_token_get_value_str(const fe_token_t *tok);

