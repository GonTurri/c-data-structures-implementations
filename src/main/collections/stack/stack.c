#include "stack.h"

t_stack *stack_create(void)
{
    t_stack *stack = malloc(sizeof(t_stack));
    stack->elements = linked_list_create();
    return stack;
}

void stack_push(t_stack *stack, void *elem)
{
    linked_list_add_first(stack->elements, elem);
}

t_stack_error stack_pop(t_stack *stack, void **out_buffer)
{
    if (stack_is_empty(stack))
        return STACK_EMPTY;

    linked_list_remove(stack->elements, 0, out_buffer);

    return STACK_SUCCESS;
}

t_stack_error stack_peek(t_stack *stack, void **out_buffer)
{
    if (stack_is_empty(stack))
        return STACK_EMPTY;
    linked_list_get(stack->elements, 0, out_buffer);
    return STACK_SUCCESS;
}

// int stack_search(t_stack* stack, void* elem){
//     linked_list_find
// }

int stack_size(t_stack *stack)
{
    return linked_list_size(stack->elements);
}

bool stack_is_empty(t_stack *stack)
{
    return stack_size(stack) == 0;
}

void stack_clean(t_stack *stack)
{
    linked_list_clean(stack->elements);
}

void stack_clean_and_destroy_elements(t_stack *stack, void (*element_destroyer)(void *))
{
    linked_list_clean_and_destroy_elements(stack->elements, element_destroyer);
}

void stack_destroy(t_stack *stack)
{
    linked_list_destroy(stack->elements);
    free(stack);
}

void stack_destroy_and_destroy_elements(t_stack *stack, void (*element_destroyer)(void *))
{
    linked_list_destroy_and_destroy_elements(stack->elements, element_destroyer);
    free(stack);
}
