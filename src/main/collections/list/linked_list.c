#include "linked_list.h"

static t_double_l_node *create_element(void *data);

static bool should_traverse_backwards(t_linked_list *list, int index);

static t_double_l_node *list_traverse_forward(t_linked_list *list, int index);

static t_double_l_node *list_traverse_backwards(t_linked_list *list, int index);

static t_list_error list_internal_get(t_linked_list *list, int index, t_double_l_node **result);

static t_list_error list_internal_find(t_linked_list *list, bool (*condition)(void *), t_double_l_node **result);

static bool index_out_of_bounds(t_linked_list *list, int index);

static void destroy_node(t_double_l_node *node);

static void linked_list_remove_element(t_linked_list *list, t_double_l_node *element);

static void adjust_tail(t_linked_list *list);

static t_double_l_node *split_linked_list(t_double_l_node *head);

static t_double_l_node *merge_linked_lists(t_double_l_node *a, t_double_l_node *b, bool (*comparator)(void *, void *));

static t_double_l_node *merge_sort(t_double_l_node *head, bool (*comparator)(void *, void *));

static void add_element_in_front_of(t_linked_list *list, void *data, t_double_l_node *node);

static void add_element_behind(t_linked_list *list, void *data, t_double_l_node *node);

static void *linked_list_internal_foldl(t_double_l_node *head, void *seed, void *(*operation)(void *, void *));

static void *linked_list_internal_foldr(t_double_l_node *tail, void *seed, void *(*operation)(void *, void *));

t_linked_list *linked_list_create(void)
{
    t_linked_list *list = malloc(sizeof(t_linked_list));
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

int linked_list_size(t_linked_list *list)
{
    return list->size;
}

bool linked_list_is_empty(t_linked_list *list)
{
    return linked_list_size(list) == 0;
}

// al final
void linked_list_add(t_linked_list *list, void *elem)
{
    t_double_l_node *node = create_element(elem);
    if (linked_list_is_empty(list))
    {
        list->head = list->tail = node;
        list->size++;
        return;
    }

    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
    list->size++;
    return;
}

void linked_list_add_first(t_linked_list *list, void *elem)
{
    t_double_l_node *node = create_element(elem);
    node->next = list->head;
    if (!linked_list_is_empty(list))
    {
        list->head->prev = node;
    }
    list->head = node;
    list->size++;
    return;
}

t_list_error linked_list_add_to_index(t_linked_list *list, int index, void *elem)
{
    if (0 == index)
    {
        linked_list_add_first(list, elem);
        return LIST_SUCCESS;
    }

    if (index == (linked_list_size(list) - 1))
    {
        linked_list_add(list, elem);
        return LIST_SUCCESS;
    }

    t_double_l_node *temp;
    t_list_error err = list_internal_get(list, index - 1, &temp);

    if (err != LIST_SUCCESS)
        return err;

    add_element_in_front_of(list, elem, temp);

    return err;
}

void linked_list_add_all(t_linked_list *self, t_linked_list *other)
{
    t_double_l_node *temp = self->head;
    while (temp)
    {
        linked_list_add(other, temp->data);
        temp = temp->next;
    }

    return;
}

int linked_list_add_sorted(t_linked_list *list, void *data, bool (*comparator)(void *, void *))
{
    int index = 0;
    if (linked_list_is_empty(list))
    {
        linked_list_add_first(list, data);
        return index;
    }
    t_double_l_node *temp = list->head;
    while (temp)
    {
        if (!comparator(temp->data, data))
        {
            if (temp == list->head)
            {
                linked_list_add_first(list, data);
                return index;
            }
            add_element_behind(list, data, temp);

            return index;
        }
        temp = temp->next;
        index++;
    }

    linked_list_add(list, data);
    return index - 1;
}

t_list_error linked_list_get(t_linked_list *list, int index, void **buffer)
{
    t_double_l_node *temp;
    t_list_error result_error = list_internal_get(list, index, &temp);
    if (result_error != LIST_SUCCESS)
    {
        return result_error;
    }

    *buffer = temp->data;

    return result_error;
}

t_list_error linked_list_set(t_linked_list *list, int index, void *new_value, void **old_value)
{
    t_double_l_node *temp;
    t_list_error result_error = list_internal_get(list, index, &temp);
    if (result_error != LIST_SUCCESS)
    {
        return result_error;
    }

    if (old_value)
        *old_value = temp->data;
    temp->data = new_value;

    return result_error;
}

t_list_error linked_list_set_and_destroy_element(t_linked_list *list, int index, void *new_value, void (*element_destroyer)(void *))
{
    void *old_value;
    t_list_error err = linked_list_set(list, index, new_value, &old_value);
    if (err != LIST_SUCCESS)
        return err;

        element_destroyer(old_value);

    return err;
}

t_list_error linked_list_replace_by_condition(t_linked_list *list, bool (*condition)(void *), void *new_value, void **old_value)
{
    t_double_l_node *temp;
    t_list_error result_error = list_internal_find(list, condition, &temp);
    if (result_error != LIST_SUCCESS)
    {
        return result_error;
    }

    if (old_value)
        *old_value = temp->data;
    temp->data = new_value;

    return result_error;
}

t_list_error linked_list_replace_and_destroy_by_condition(t_linked_list *list, bool (*condition)(void *), void *new_value, void (*element_destroyer)(void *))
{
    void *old_value;
    t_list_error err = linked_list_replace_by_condition(list, condition, new_value, &old_value);
    if (err != LIST_SUCCESS)
        return err;

        element_destroyer(old_value);

    return err;
}

t_list_error linked_list_remove(t_linked_list *list, int index, void **buffer)
{
    t_double_l_node *to_delete;
    t_list_error result_error = list_internal_get(list, index, &to_delete);

    if (result_error != LIST_SUCCESS)
        return result_error;

    if (to_delete)
    {
        if (buffer)
            *buffer = to_delete->data;
        linked_list_remove_element(list, to_delete);
    }

    return result_error;
}

t_list_error linked_list_remove_and_destroy(t_linked_list *list, int index, void (*element_destroyer)(void *))
{
    t_double_l_node *to_delete;
    t_list_error result_error = list_internal_get(list, index, &to_delete);

    if (result_error != LIST_SUCCESS)
        return result_error;

    if (to_delete)
    {
        element_destroyer(to_delete->data);
        linked_list_remove_element(list, to_delete);
    }

    return result_error;
}

t_list_error linked_list_remove_by_condition(t_linked_list *list, bool (*condition)(void *), void **buffer)
{

    t_double_l_node *to_delete;
    t_list_error err = list_internal_find(list, condition, &to_delete);

    if (err != LIST_SUCCESS)
    {
        return err;
    }

    if (to_delete)
    {
        if (buffer)
            *buffer = to_delete->data;
        linked_list_remove_element(list, to_delete);
    }

    return err;
}

t_list_error linked_list_remove_and_destroy_by_condition(t_linked_list *list, bool (*condition)(void *), void (*element_destroyer)(void *))
{

    t_double_l_node *to_delete;
    t_list_error err = list_internal_find(list, condition, &to_delete);

    if (err != LIST_SUCCESS)
        return err;

    if (to_delete)
    {
        element_destroyer(to_delete->data);
        linked_list_remove_element(list, to_delete);
    }

    return err;
}

void linked_list_foreach(t_linked_list *list, void (*closure)(void *))
{
    t_double_l_node *temp = list->head;
    while (temp)
    {
        closure(temp->data);
        temp = temp->next;
    }

    return;
}

t_list_error linked_list_find(t_linked_list *list, bool (*condition)(void *), void **buffer)
{
    t_double_l_node *temp;
    t_list_error err = list_internal_find(list, condition, &temp);
    if (err != LIST_SUCCESS)
    {
        return err;
    }

    *buffer = temp ? temp->data : NULL;

    return err;
}

int linked_list_index_of(t_linked_list* list, void* elem){
    int index = 0;
    t_double_l_node* temp = list->head;
    while (temp)
    {  
        if(temp->data == elem) return index;
        temp = temp->next;
        index++;
    }

    return -1;
    
}

void linked_list_clean(t_linked_list *list)
{
    while (linked_list_size(list) > 0)
    {
        linked_list_remove(list, 0, NULL);
    }
}

void linked_list_clean_and_destroy_elements(t_linked_list *list, void (*element_destroyer)(void *))
{
    while (linked_list_size(list) > 0)
    {
        linked_list_remove_and_destroy(list, 0, element_destroyer);
    }
}

bool linked_list_any_satisfy(t_linked_list *list, bool (*condition)(void *))
{
    return list_internal_find(list, condition, NULL) == LIST_SUCCESS;
}

int linked_list_count(t_linked_list *list, bool (*condition)(void *))
{
    int cont = 0;
    t_double_l_node *temp = list->head;
    while (temp)
    {
        if (condition(temp->data))
            cont++;
        temp = temp->next;
    }
    return cont;
}

bool linked_list_all_satisfy(t_linked_list *list, bool (*condition)(void *))
{
    t_double_l_node *temp = list->head;
    while (temp)
    {
        if (!condition(temp->data))
            return false;
        temp = temp->next;
    }
    return true;
}

void linked_list_destroy(t_linked_list *list)
{
    linked_list_clean(list);
    free(list);
}

void linked_list_destroy_and_destroy_elements(t_linked_list *list, void (*element_destroyer)(void *))
{
    linked_list_clean_and_destroy_elements(list, element_destroyer);
    free(list);
}

t_linked_list *linked_list_duplicate(t_linked_list *list)
{
    t_linked_list *dup = linked_list_create();
    linked_list_add_all(list, dup);
    return dup;
}

t_linked_list *linked_list_filter(t_linked_list *list, bool (*condition)(void *))
{
    t_linked_list *result = linked_list_create();
    t_double_l_node *temp = list->head;
    while (temp)
    {
        if (condition(temp->data))
        {
            linked_list_add(result, temp->data);
        }
        temp = temp->next;
    }
    return result;
}

t_linked_list *linked_list_map(t_linked_list *list, void *(*mapper)(void *))
{
    t_linked_list *result = linked_list_create();
    t_double_l_node *temp = list->head;
    while (temp)
    {
        linked_list_add(result, mapper(temp->data));
        temp = temp->next;
    }
    return result;
}

void linked_list_sort(t_linked_list *list, bool (*comparator)(void *, void *))
{
    if (!list)
        return;

    list->head = merge_sort(list->head, comparator);

    adjust_tail(list);
}

t_linked_list *linked_list_sorted(t_linked_list *list, bool (*comparator)(void *, void *))
{
    t_linked_list *sorted = linked_list_duplicate(list);
    linked_list_sort(sorted, comparator);
    return sorted;
}

t_linked_list *linked_list_slice(t_linked_list *list, int start, int count)
{
    t_linked_list *result = linked_list_create();
    t_double_l_node *temp;
    t_list_error err = list_internal_get(list, start, &temp);
    if (err != LIST_SUCCESS)
    {
        return result;
    }

    for (int i = 0; i < count && temp; i++)
    {
        linked_list_add(result, temp->data);
        temp = temp->next;
    }

    return result;
}

t_linked_list *linked_list_slice_and_remove(t_linked_list *list, int start, int count)
{
    t_linked_list *result = linked_list_create();
    t_double_l_node *temp, *next;
    t_list_error err = list_internal_get(list, start, &temp);
    if (err != LIST_SUCCESS)
    {
        return result;
    }

    for (int i = 0; i < count && temp; i++)
    {
        linked_list_add(result, temp->data);
        next = temp->next;
        linked_list_remove_element(list, temp);
        temp = next;
    }

    return result;
}

t_linked_list *linked_list_take(t_linked_list *origin, int count)
{
    return linked_list_slice(origin, 0, count);
}

t_linked_list *linked_list_take_and_remove(t_linked_list *origin, int count)
{
    return linked_list_slice_and_remove(origin, 0, count);
}

t_linked_list *linked_list_drop(t_linked_list *origin, int count)
{
    if (count == 0)
        return linked_list_duplicate(origin);

    return linked_list_slice(origin, count, linked_list_size(origin) - count);
}

t_linked_list *linked_list_drop_and_remove(t_linked_list *origin, int count)
{
    if (count == 0)
    {
        t_linked_list *res = linked_list_duplicate(origin);
        linked_list_clean(origin);
        return res;
    }

    return linked_list_slice_and_remove(origin, count, linked_list_size(origin) - count);
}

void *linked_list_foldl(t_linked_list *list, void *seed, void *(*operation)(void *, void *))
{
    return linked_list_internal_foldl(list->head, seed, operation);
}

void *linked_list_foldl1(t_linked_list *list, void *(*operation)(void *, void *))
{
    if (linked_list_size(list) == 0)
        return NULL;
    return linked_list_internal_foldl(list->head->next, list->head->data, operation);
}

void *linked_list_foldr(t_linked_list *list, void *seed, void *(*operation)(void *, void *))
{
    return linked_list_internal_foldr(list->tail, seed, operation);
}

void *linked_list_foldr1(t_linked_list *list, void *(*operation)(void *, void *))
{
    if (linked_list_size(list) == 0)
        return NULL;
    return linked_list_internal_foldr(list->tail->prev, list->tail->data, operation);
}

// folds
// min = max

static t_double_l_node *list_traverse_forward(t_linked_list *list, int index)
{
    t_double_l_node *temp = list->head;
    for (int i = 0; i < index; i++)
    {
        temp = temp->next;
    }
    return temp;
}

static t_double_l_node *list_traverse_backwards(t_linked_list *list, int index)
{
    t_double_l_node *temp = list->tail;
    for (int i = list->size - 1; i > index; i--)
    {
        temp = temp->prev;
    }
    return temp;
}

static t_list_error list_internal_get(t_linked_list *list, int index, t_double_l_node **result)
{
    if (index_out_of_bounds(list, index))
        return LIST_INDEX_OUT_OF_BOUNDS;
    *result = should_traverse_backwards(list, index) ? list_traverse_backwards(list, index) : list_traverse_forward(list, index);
    return LIST_SUCCESS;
}

static t_list_error list_internal_find(t_linked_list *list, bool (*condition)(void *), t_double_l_node **result)
{
    t_double_l_node *temp = list->head;
    while (temp)
    {
        if (condition(temp->data))
        {
            if (result)
                *result = temp;
            return LIST_SUCCESS;
        }
        temp = temp->next;
    }
    return LIST_NOT_FOUND;
}

static bool should_traverse_backwards(t_linked_list *list, int index)
{
    return index >= (linked_list_size(list) / 2);
}

static t_double_l_node *create_element(void *data)
{
    t_double_l_node *node = malloc(sizeof(t_double_l_node));
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

static void destroy_node(t_double_l_node *node)
{
    free(node);
}

static void add_element_in_front_of(t_linked_list *list, void *data, t_double_l_node *node)
{
    t_double_l_node *new = create_element(data);

    new->prev = node;
    new->next = node->next;

    node->next = new;
    if (new->next)
    {
        new->next->prev = new;
    }

    list->size++;
}

static void add_element_behind(t_linked_list *list, void *data, t_double_l_node *node)
{

    t_double_l_node *new = create_element(data);
    new->prev = node->prev;
    new->next = node;

    node->prev = new;

    if (new->prev)
    {
        new->prev->next = new;
    }

    list->size++;
}

static void linked_list_remove_element(t_linked_list *list, t_double_l_node *element)
{

    if (linked_list_size(list) == 1)
    {
        // list of only one element
        list->head = list->tail = NULL;
    }

    else if (NULL == element->prev)
    {
        // removing from head
        list->head = list->head->next;
        if (list->head)
            list->head->prev = NULL;
    }

    else if (NULL == element->next)
    {
        // removing from tail
        list->tail = list->tail->prev;
        if (list->tail)
            list->tail->next = NULL;
    }

    else
    {

        // element in the middle
        element->next->prev = element->prev;
        element->prev->next = element->next;
    }

    list->size--;
    destroy_node(element);
}

static bool index_out_of_bounds(t_linked_list *list, int index)
{
    return index >= linked_list_size(list) || index < 0;
}

static void adjust_tail(t_linked_list *list)
{
    list->tail = list->head;
    while (list->tail && list->tail->next)
    {
        list->tail = list->tail->next;
    }
}

static t_double_l_node *split_linked_list(t_double_l_node *head)
{
    t_double_l_node *slow = head;
    t_double_l_node *fast = head;

    while (fast && fast->next && fast->next->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    t_double_l_node *back = slow->next;
    slow->next = NULL;
    if (back)
        back->prev = NULL;
    return back;
}

static t_double_l_node *merge_linked_lists(t_double_l_node *a, t_double_l_node *b, bool (*comparator)(void *, void *))
{
    if (!a)
        return b;
    if (!b)
        return a;

    if (comparator(a->data, b->data))
    {
        // a before b
        a->next = merge_linked_lists(a->next, b, comparator);
        if (a->next)
            a->next->prev = a;
        return a;
    }
    b->next = merge_linked_lists(a, b->next, comparator);
    if (b->next)
        b->next->prev = b;
    return b;
}

static t_double_l_node *merge_sort(t_double_l_node *head, bool (*comparator)(void *, void *))
{
    if (!head || !head->next)
        return head;

    t_double_l_node *back = split_linked_list(head);
    head = merge_sort(head, comparator);
    back = merge_sort(back, comparator);

    return merge_linked_lists(head, back, comparator);
}

static void *linked_list_internal_foldl(t_double_l_node *head, void *seed, void *(*operation)(void *, void *))
{
    void *acc = seed;
    while (head)
    {
        acc = operation(acc, head->data);
        head = head->next;
    }

    return acc;
}

static void *linked_list_internal_foldr(t_double_l_node *tail, void *seed, void *(*operation)(void *, void *))
{
    void *acc = seed;
    while (tail)
    {
        acc = operation(tail->data, acc);
        tail = tail->prev;
    }

    return acc;
}

