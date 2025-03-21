#include "queue.h"

t_queue *queue_create(void)
{
    t_queue *queue = malloc(sizeof(t_queue));
    queue->elements = linked_list_create();
    return queue;
}

void queue_push(t_queue *queue, void *elem)
{
    linked_list_add(queue->elements, elem);
}

t_queue_error queue_pop(t_queue *queue, void **out_buffer)
{
    if (queue_is_empty(queue))
        return QUEUE_EMPTY;
    linked_list_remove(queue->elements, 0, out_buffer);
    return QUEUE_SUCCESS;
}

t_queue_error queue_peek(t_queue *queue, void **out_buffer)
{
    if (queue_is_empty(queue))
        return QUEUE_EMPTY;
    linked_list_get(queue->elements, 0, out_buffer);
    return QUEUE_SUCCESS;
}

int queue_size(t_queue *queue)
{
    return linked_list_size(queue->elements);
}

bool queue_is_empty(t_queue *queue)
{
    return queue_size(queue) == 0;
}

void queue_clean(t_queue *queue)
{
    linked_list_clean(queue->elements);
}

void queue_clean_and_destroy_elements(t_queue *queue, void (*element_destroyer)(void *))
{
    linked_list_clean_and_destroy_elements(queue->elements, element_destroyer);
}

void queue_destroy(t_queue *queue)
{
    linked_list_destroy(queue->elements);
    free(queue);
}

void queue_destroy_and_destroy_elements(t_queue *queue, void (*element_destroyer)(void *))
{
    linked_list_destroy_and_destroy_elements(queue->elements, element_destroyer);
    free(queue);
}
