#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include "../list/linked_list.h"

typedef enum{
    QUEUE_SUCCESS = 0,
    QUEUE_EMPTY
} t_queue_error;


typedef struct {
    t_linked_list* elements;
} t_queue;

t_queue* queue_create(void);

void queue_push(t_queue* queue,void* elem);

t_queue_error queue_pop(t_queue* queue, void** out_buffer);

t_queue_error queue_peek(t_queue* queue, void** out_buffer);

int queue_size(t_queue* queue);

bool queue_is_empty(t_queue* queue);

void queue_clean(t_queue* queue);

void queue_clean_and_destroy_elements(t_queue* queue,void(*element_destroyer)(void*));

void queue_destroy(t_queue* queue);

void queue_destroy_and_destroy_elements(t_queue* queue, void(*element_destroyer)(void*));



#endif

