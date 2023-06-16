#include <stdlib.h>
#include <ctype.h>
#include <foam/core/config/tokenizer.h>
#include <foam/core/ext/io.h>
#include <foam/core/ext/logger.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/string.h>

#define FE_SCRIPT_FILE_EXT "foam"

#define CURR  (line[i])
#define CURRP (line[*i])
#define PREV  (fe_vector_count(line) > 0 && i - 1 >= 0 ? line[i - 1]  : 0)
#define PREVP (fe_vector_count(line) > 0 && (*i > 0)   ? line[*i - 1] : 0)
#define NEXT  (i + 1 < line_len ? line[i + 1]  : 0)
#define NEXTP (*i + 1 < line_len ? line[*i + 1] : 0)

#define NEXTP_VALID(line_len) (*i < line_len - 1)

static char *token_str[] =
{
    "invalid",

    "identifier",
    "number",
    "string",
 
    ".",
    "!",

    "{",
    "}",

    "(",
    ")",

};

static bool 
valid_num_char(char c)
{
    return 
    (
        (c >= '0' && c <= '9') ||
        (c >= 'a' && c <= 'f') ||
        (c >= 'A' && c <= 'F') ||
        c == '.'               ||
        c == 'o'  || c == 'O'  ||
        c == 'x'  || c == 'X'  || 
        c == '-'
    );
}

static bool
valid_name_char(char c)
{
    return c == '_' || c == '!' || c == '-' || c == '?' ;
}

static char*
grab_name(const char *line, size_t line_len, size_t *i)
{
    char buf[4096];
    memset(buf, '\0', 4096);
    size_t pos = 0;

    buf[pos++] = line[*i];
    *i += 1;

    while (*i < line_len) 
    {
        if (!isalnum(CURRP) && !valid_name_char(CURRP) && CURRP) 
        {
            break;
        }
        
        buf[pos++] = line[*i];
        *i += 1;
    }
    
    char *r = malloc(strlen(buf)+1);
    strcpy(r, buf);

    return r;
}

static char*
grab_num(const char *line, size_t line_len, size_t *i)
{
    char buf[4096];
    memset(buf, '\0', 4096);
    size_t pos = 0;

    FE_TODO("Add error handling (checking for more than 1 dot, too large of numbers, etc)");
    FE_TODO("Add checking of exact max and min of integer types, and set appropriately");
    
    buf[pos++] = line[*i];
    *i += 1;
    while (*i < line_len && valid_num_char(CURRP)) 
    {
        const char cur = CURRP;

        buf[pos++] = cur;
        *i += 1;
    }
    
    char *r = malloc(strlen(buf)+1);
    strcpy(r, buf);

    return r;
}

FE_TOKEN_TYPE
grab_op(const char *line, size_t *i)
{
    FE_TOKEN_TYPE type = FE_TOKEN_INVALID;

    const char cur = CURRP;

    if      (cur == '{')  type = FE_TOKEN_FUNC_OPEN;
    else if (cur == '}')  type = FE_TOKEN_FUNC_CLOSE;
    else if (cur == '(')  type = FE_TOKEN_LIST_OPEN;
    else if (cur == ')')  type = FE_TOKEN_LIST_CLOSE;

    else if (cur == '.')  type = FE_TOKEN_ARG;
    else if (cur == '!')  type = FE_TOKEN_DEFINE;

    *i += 1;

    return type;
}

char*
grab_const_str(const char *line, size_t line_len, size_t line_num, size_t *i)
{
    size_t start     = *i;
    const char quote = CURRP;

    FE_TODO("Grabbing const str grab_const_str .... This should be unlimited");

    char buf[4096];
    memset(buf, '\0', 4096);
    size_t pos = 0;
    
    *i += 1;

    while (*i < line_len) 
    {
        const char cur = CURRP;

        if (cur == quote) break;

        buf[pos++] = cur;
        *i += 1;
    }

    if (CURRP != quote)
    {
        *i = start;
        fe_log("[Line %u] tokenizer - quote missing close token [%c]", line_num, quote);
        return NULL;
    }

    *i += 1;
     
    char *r = malloc(strlen(buf)+1);
    strcpy(r, buf);

    return r;

}

FE_RETURN_CODE
tokenize(fe_token_list_t *tl, const char *line, const size_t line_len, size_t line_num)
{
    size_t i = 0;

    while (i < line_len)
    {
        size_t col = i;

        //handle blank spaces
        if (isspace(CURR))
        {
            i++;
            continue;
        }
        //comments
        else if (CURR == '#') 
        {
            FE_TODO("Add support for multi-block comments?");
            break;
        }
        //handle idens
        else if (isalpha(CURR) || CURR == '_')
        {
            char *name = grab_name(line, line_len, &i);
            FE_TOKEN_TYPE type = FE_TOKEN_IDEN;
            fe_token_list_make_token(tl, type, name, strlen(name), line_num, col);
            continue;
        }
        //handle numbers
        else if ((CURR == '-' && isdigit(NEXT)) || isdigit(CURR))
        {
            char *num = grab_num(line, line_len, &i);
            fe_token_list_make_token(tl, FE_TOKEN_NUM, num, strlen(num), line_num, col);
            continue;
        }

        //handle strings
        else if(CURR == '\"' || CURR == '\'') 
        {
            char *str = grab_const_str(line, line_len, line_num, &i);
            if (str)
            {
                fe_token_list_make_token(tl, FE_TOKEN_STR, str, strlen(str), line_num, col);
            }
            else
            {
                return FE_ERR;
            }
        }
        else
        {
            //handle operators
            FE_TOKEN_TYPE op_type = grab_op(line, &i);
            if (op_type == FE_TOKEN_INVALID)
            {
                fe_log("[Line %u] tokenizer - invalid symbol '%c'", line_num, line[col]);
                return FE_ERR;
            }

            fe_token_list_make_token(tl, op_type, NULL, 0, line_num, col);
        }

    }

    return FE_OK;
}

fe_vec(char*)
load_source(const char *path, FE_RETURN_CODE *rc)
{
    fe_vec(char*) lines = FE_VEC_EMPTY;

    FILE *file = fopen(path, "r"); 
    if (file == NULL)
    {
        fe_log_warn("Failed to load '%s' for tokenizing", path);
        *rc = FE_ERR_PATH_NOT_FOUND;
        return NULL;
    }

    char line[4096];
    while (fgets(line, sizeof(line), file)) 
    {
        char *l = strdup(line);
        fe_vec_add(lines, l);
    }
    
    *rc = FE_OK;

	return lines;
}

void
fe_token_list_make_token(fe_token_list_t *tl, FE_TOKEN_TYPE type, char *data, size_t data_len, size_t line, size_t col)
{
    fe_token_t *tok = NULL;
    fe_vec_add_and_get(tl->tokens, tok);

    tok->type         = type;
    tok->str_data     = data;
    tok->str_data_len = data_len;
    tok->num_data     = 0;
    tok->line_num     = line;
    tok->col          = col; 

    if (tok->type == FE_TOKEN_NUM)
    {
        char *end = NULL;
        f64 res = strtod(tok->str_data, &end);
        if (end != tok->str_data)
        {
            tok->num_data = res;
        }
    }

}

void           
fe_token_list_push_number(fe_token_list_t *tl, const char *num)
{
    char *dup = strdup(num);
    fe_token_list_make_token(tl, FE_TOKEN_NUM, dup, strlen(dup), 0, 0);
}

void           
fe_token_list_push_string(fe_token_list_t *tl, const char *str)
{
    char *dup = strdup(str);
    fe_token_list_make_token(tl, FE_TOKEN_STR, dup, strlen(dup), 0, 0);
}

void           
fe_token_list_push_token(fe_token_list_t *tl, const fe_token_t *tok)
{
    fe_token_list_make_token(tl, tok->type, strdup(tok->str_data), tok->str_data_len, 0, 0);
}

void
fe_token_list_make(fe_token_list_t *tl)
{
    tl->tokens = FE_VEC_EMPTY;
    tl->lines  = FE_VEC_EMPTY;
    tl->path   = NULL;
}

void 
fe_token_list_kill(fe_token_list_t *tl)
{
    fe_vec_for_each(tl->tokens, tok)
    {
        free(tok->str_data);
    }

    fe_vec_kill(tl->tokens, NULL); 

    fe_vec_for_each(tl->lines, line)
    {
        free(*line);
    }
    fe_vec_kill(tl->lines, NULL);

    if (tl->path) free(tl->path);
}

FE_RETURN_CODE 
fe_token_list_make_from_file(fe_token_list_t *tl, const char *path)
{
    FE_RETURN_CODE res = FE_OK;

    fe_token_list_make(tl);

    tl->lines = load_source(path, &res);
    if (tl->lines == NULL)
    {
        res = FE_ERR_PATH_NOT_FOUND;
        goto error;
    }

    tl->path  = strdup(path);
    
    if (res != FE_OK) goto error;

    fe_vec_for_each(tl->lines, l)
    {        
        res = tokenize(tl, *l, strlen(*l), _fe_for_each_index + 1);

        if (res != FE_OK) goto error;
    }

    return res;

error:
    fe_token_list_kill(tl);
    return res;
}

FE_RETURN_CODE  
fe_token_list_make_from_memory(fe_token_list_t *tl, const char *data, size_t data_len)
{
    FE_RETURN_CODE res = FE_OK;

    fe_token_list_make(tl);

    tl->path         = NULL;
    tl->lines        = FE_VEC_EMPTY;
    res = tokenize(tl, data, data_len, 0);

    if (res != FE_OK)
    {
        fe_token_list_kill(tl);
        tl = NULL;
    }

    return res;
}

u32
fe_token_list_get_count(const fe_token_list_t *tl)
{
    return fe_vec_get_count(tl->tokens);
}

fe_token_t*          
fe_token_list_get_at(const fe_token_list_t *tl, u32 index)
{
    if (index >= fe_vec_get_count(tl->tokens)) return NULL;
    return &tl->tokens[index];
}

void
fe_token_list_print(const fe_token_list_t *tl)
{
    for (u32 i = 0; i < fe_token_list_get_count(tl); i++)
    {    
        fe_token_t *tok = fe_token_list_get_at(tl, i);
        fe_token_print(tok);
        printf("\n");
    }
}

void
fe_token_print(const fe_token_t *tok)
{
    if (tok->type == FE_TOKEN_IDEN || 
        tok->type == FE_TOKEN_STR   )
    {
        printf("%s", tok->str_data);
    }
    else if (tok->type == FE_TOKEN_NUM)
    {
        printf("%f", tok->num_data);
    }
    else 
    {
        printf("%s", token_str[tok->type]);
    }
}

void           
fe_token_copy(fe_token_t *dst, const fe_token_t *src)
{
    dst->type         = src->type;
    if (dst->str_data) free(dst->str_data);
    dst->str_data     = strdup(src->str_data);
    dst->str_data_len = src->str_data_len;
    dst->num_data     = src->num_data;
    dst->line_num     = src->line_num;
    dst->col          = src->col;
}

bool           
fe_token_is_equal(const fe_token_t *tok, const fe_token_t *other)
{
    if (tok->type == other->type)
    {
        if (tok->type == FE_TOKEN_NUM) 
            return tok->num_data == other->num_data;

        else if (tok->type == FE_TOKEN_STR || tok->type == FE_TOKEN_IDEN)
            return STR_EQ(tok->str_data, other->str_data);

        else return true;
    }

    return false;
}

const char*
fe_token_get_type_str(FE_TOKEN_TYPE type)
{
    return token_str[type];
}

const char*
fe_token_get_value_str(const fe_token_t *tok)
{
    if (tok->type == FE_TOKEN_IDEN || 
        tok->type == FE_TOKEN_NUM  || 
        tok->type == FE_TOKEN_STR  )
    {
        return tok->str_data;
    }
    else 
    {
        return token_str[tok->type];
    }

    return "";
}
