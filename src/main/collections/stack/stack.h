#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "../list/linked_list.h"

typedef enum
{
    STACK_SUCCESS = 0,
    STACK_EMPTY
} t_stack_error;

typedef struct
{
    t_linked_list *elements;
} t_stack;

t_stack *stack_create(void);

void stack_push(t_stack *stack, void *elem);

t_stack_error stack_pop(t_stack *stack, void **out_buffer);

t_stack_error stack_peek(t_stack *stack, void **out_buffer);

int stack_search(t_stack *stack, void *elem);

int stack_size(t_stack *stack);

bool stack_is_empty(t_stack *stack);

void stack_clean(t_stack *stack);

void stack_clean_and_destroy_elements(t_stack *stack, void (*element_destroyer)(void *));

void stack_destroy(t_stack *stack);

void stack_destroy_and_destroy_elements(t_stack *stack, void (*element_destroyer)(void *));

#endif
