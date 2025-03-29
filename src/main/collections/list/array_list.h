#ifndef ARRAY_LIST_H_INCLUDED
#define ARRAY_LIST_H_INCLUDED

#include <stdlib.h>
#include "list_error.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define BASE_CAPACITY 10

#define CAPACITY_MULTIPLIER 2

typedef struct
{
    unsigned int capacity;
    unsigned int element_count;
    void **array;
} t_array_list;

t_array_list *array_list_create(void);

t_array_list *array_list_create_with_capacity(unsigned int capacity);

void array_list_foreach(t_array_list *self, void (*operation)(void *));

void array_list_clean(t_array_list *self);

void array_list_clean_and_destroy_elements(t_array_list *self, void (*element_destroyer)(void *));

void array_list_destroy(t_array_list *self);

void array_list_destroy_and_destroy_elements(t_array_list *self, void (*element_destroyer)(void *));

unsigned int array_list_size(t_array_list *self);

bool array_list_is_empty(t_array_list *self);

t_list_error array_list_get(t_array_list *self, int index, void **out_buffer);

void array_list_add(t_array_list *self, void *data);

t_list_error array_list_add_to_index(t_array_list *self, int index, void *data);

t_list_error array_list_remove(t_array_list *self, int index, void **deleted);

t_list_error array_list_remove_and_destroy(t_array_list *self, int index, void (*element_destroyer)(void *));

t_list_error array_list_remove_element(t_array_list *self, void *to_delete);

//TODO: Implement all same methods as linked lists

#endif
