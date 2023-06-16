#include <stdlib.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/logger.h>
#include <foam/core/ext/stack.h>

#define DEFAULT_STACK_SIZE 100

FE_RETURN_CODE
fe_stack_make(fe_stack_t *s)
{
    s->capacity = DEFAULT_STACK_SIZE;
    s->data     = malloc(sizeof(*s->data) * s->capacity);
    s->top      = 0;
    s->empty    = true;

    return FE_OK;
}

void
fe_stack_kill(fe_stack_t *s)
{
    free(s->data);
}

bool
fe_stack_is_empty(fe_stack_t *s)
{
    return s->empty;
}

bool
fe_stack_is_full(fe_stack_t *s)
{
    return s->top == s->capacity;
}

size_t
fe_stack_get_capacity(fe_stack_t *s)
{
    return s->capacity;
}

size_t
fe_stack_top(fe_stack_t *s)
{
    return s->top;
}

u32
fe_stack_peek(fe_stack_t *s)
{
    if (s->empty)
    {
        fe_log_warn("Failed to peek, stack is full");
        return 0;
    }

    return s->data[s->top-1];
}

void
fe_stack_push(fe_stack_t *s, u32 val)
{
    if (fe_stack_is_full(s)) 
    {
        u32 *tmp = realloc(s->data, sizeof(u32) * (s->capacity * 2));
        if (tmp)
        {
            s->data = tmp;
            s->capacity *= 2;
        }
    } 

    s->empty = false;
    s->data[s->top++] = val;
}

u32
fe_stack_pop(fe_stack_t *s)
{
    if (s->empty)
    {
        fe_log_fatal("Failed to pop, stack is empty");
        return (u32)-1;
    }

    if (s->top == 1) s->empty = true;
    return s->data[--s->top];
}

bool
fe_stack_contains(fe_stack_t *s, u32 val)
{
    for (size_t i = 0; i < s->top; i++)
    {
        if (s->data[i] == val) return true;
    }
        
    return false;
}
