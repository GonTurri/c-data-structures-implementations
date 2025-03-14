#ifndef LINK_LIST_H_INCLUDED
#define LINK_LIST_H_INCLUDED

#include "node.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    int size;
    t_double_l_node *head;
    t_double_l_node *tail;
} t_linked_list;

typedef enum
{
    LIST_SUCCESS = 0,
    // LIST_NULL_POINTER,
    LIST_INDEX_OUT_OF_BOUNDS,
    LIST_NOT_FOUND
} t_list_error;

// creation/deletion

t_linked_list *linked_list_create(void);

// list primitives

int linked_list_size(t_linked_list *list);

bool linked_list_is_empty(t_linked_list *list);

void linked_list_add(t_linked_list *list, void *elem);

void linked_list_add_first(t_linked_list *list, void *elem);

void linked_list_add_all(t_linked_list *self, t_linked_list *other);

t_list_error linked_list_add_to_index(t_linked_list *list, int index, void *elem);

t_list_error linked_list_get(t_linked_list *list, int index, void **buffer);

t_list_error linked_list_set(t_linked_list *list, int index, void *new_value, void **old_value);

t_list_error linked_list_set_and_destroy_element(t_linked_list *list, int index, void *new_value, void (*destroyer)(void *));

t_list_error linked_list_replace_by_condition(t_linked_list *list, bool (*condition)(void *), void *new_value, void **old_value);

t_list_error linked_list_replace_and_destroy_by_condition(t_linked_list *list, bool (*condition)(void *), void *new_value, void (*destroyer)(void *));

t_list_error linked_list_remove(t_linked_list *list, int index, void **buffer);

t_list_error linked_list_remove_and_destroy(t_linked_list *list, int index, void (*destroyer)(void *));

t_list_error linked_list_remove_by_condition(t_linked_list *list, bool (*condition)(void *), void **buffer);

t_list_error linked_list_remove_and_destroy_by_condition(t_linked_list *list, bool (*condition)(void *), void (*destroyer)(void *));

void linked_list_clean(t_linked_list *list);

void linked_list_clean_and_destroy_elements(t_linked_list *list, void (*destroyer)(void *));

t_list_error linked_list_find(t_linked_list *list, bool (*condition)(void *), void **buffer);

bool linked_list_any_satisfy(t_linked_list *list, bool (*condition)(void *));

bool linked_list_all_satisfy(t_linked_list *list, bool (*condition)(void *));

void linked_list_foreach(t_linked_list *list, void (*closure)(void *));

void linked_list_destroy(t_linked_list *list);

void linked_list_destroy_and_destroy_elements(t_linked_list *list, void (*destroyer)(void *));

t_linked_list *linked_list_duplicate(t_linked_list *list);

t_linked_list *linked_list_filter(t_linked_list *list, bool (*condition)(void *));

t_linked_list* linked_list_map(t_linked_list* list, void*(*mapper)(void*));

#endif
