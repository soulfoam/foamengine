#pragma once

#include <foam/core/ext/sugar.h>
#include <foam/core/return_code.h>

typedef struct
{
    u32    *data;
    size_t capacity;
    size_t top;
    bool   empty;
} fe_stack_t;

FE_RETURN_CODE fe_stack_make(fe_stack_t *s);
void           fe_stack_kill(fe_stack_t *s);
bool           fe_stack_is_empty(fe_stack_t *s);
bool           fe_stack_is_full(fe_stack_t *s);
size_t         fe_stack_get_capacity(fe_stack_t *s);
size_t         fe_stack_top(fe_stack_t *s);
u32            fe_stack_peek(fe_stack_t *s);
void           fe_stack_push(fe_stack_t *s, u32 val);
u32            fe_stack_pop(fe_stack_t *s);
bool           fe_stack_contains(fe_stack_t *s, u32 val);
