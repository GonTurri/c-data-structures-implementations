#ifndef LINK_LIST_H_INCLUDED
#define LINK_LIST_H_INCLUDED

#include "node.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "list_error.h"

typedef struct
{
    int size;
    t_double_l_node *head;
    t_double_l_node *tail;
} t_linked_list;

// creation/deletion

t_linked_list *linked_list_create(void);

// list primitives

int linked_list_size(t_linked_list *list);

bool linked_list_is_empty(t_linked_list *list);

void linked_list_add(t_linked_list *list, void *elem);

void linked_list_add_first(t_linked_list *list, void *elem);

int linked_list_add_sorted(t_linked_list *list, void *data, bool (*comparator)(void *, void *));

void linked_list_add_all(t_linked_list *self, t_linked_list *other);

t_list_error linked_list_add_to_index(t_linked_list *list, int index, void *elem);

t_list_error linked_list_get(t_linked_list *list, int index, void **buffer);

t_list_error linked_list_set(t_linked_list *list, int index, void *new_value, void **old_value);

int linked_list_index_of(t_linked_list* list, void* elem);

t_list_error linked_list_set_and_destroy_element(t_linked_list *list, int index, void *new_value, void (*element_destroyer)(void *));

t_list_error linked_list_replace_by_condition(t_linked_list *list, bool (*condition)(void *), void *new_value, void **old_value);

t_list_error linked_list_replace_and_destroy_by_condition(t_linked_list *list, bool (*condition)(void *), void *new_value, void (*element_destroyer)(void *));

t_list_error linked_list_remove(t_linked_list *list, int index, void **buffer);

t_list_error linked_list_remove_and_destroy(t_linked_list *list, int index, void (*destroyer)(void *));

t_list_error linked_list_remove_by_condition(t_linked_list *list, bool (*condition)(void *), void **buffer);

t_list_error linked_list_remove_and_destroy_by_condition(t_linked_list *list, bool (*condition)(void *), void (*destroyer)(void *));

void linked_list_clean(t_linked_list *list);

void linked_list_clean_and_destroy_elements(t_linked_list *list, void (*element_destroyer)(void *));

t_list_error linked_list_find(t_linked_list *list, bool (*condition)(void *), void **buffer);

bool linked_list_any_satisfy(t_linked_list *list, bool (*condition)(void *));

bool linked_list_all_satisfy(t_linked_list *list, bool (*condition)(void *));

int linked_list_count(t_linked_list *list, bool (*condition)(void *));

void linked_list_foreach(t_linked_list *list, void (*closure)(void *));

void linked_list_destroy(t_linked_list *list);

void linked_list_destroy_and_destroy_elements(t_linked_list *list, void (*element_destroyer)(void *));

t_linked_list *linked_list_duplicate(t_linked_list *list);

t_linked_list *linked_list_filter(t_linked_list *list, bool (*condition)(void *));

t_linked_list *linked_list_map(t_linked_list *list, void *(*mapper)(void *));

void linked_list_sort(t_linked_list *list, bool (*comparator)(void *, void *));

t_linked_list *linked_list_sorted(t_linked_list *list, bool (*comparator)(void *, void *));

t_linked_list* linked_list_slice(t_linked_list* list, int start, int count);

t_linked_list* linked_list_slice_and_remove(t_linked_list* list, int start, int count);

t_linked_list *linked_list_take(t_linked_list *origin, int count);

t_linked_list *linked_list_take_and_remove(t_linked_list *origin, int count);

t_linked_list* linked_list_drop(t_linked_list* origin, int count);

t_linked_list* linked_list_drop_and_remove(t_linked_list* origin, int count);

void* linked_list_foldl(t_linked_list* list, void* seed, void*(*operation)(void*,void*));

void* linked_list_foldl1(t_linked_list* list,void*(*operation)(void*,void*));

void* linked_list_foldr(t_linked_list* list, void* seed, void*(*operation)(void*,void*));

void* linked_list_foldr1(t_linked_list* list,void*(*operation)(void*,void*));

#endif
