#include "array_list.h"

static bool index_out_of_bounds(t_array_list *self, int index);

static size_t array_current_size(t_array_list *self);

static void resize_array(t_array_list *self);

static bool array_needs_resizing(t_array_list *self);

static void shif_elements_to_right(t_array_list *self, int start);

static void shift_elements_to_left(t_array_list *self, int end);

static t_list_error remove_element(t_array_list *self, int index, void **deleted, void(*element_destroyer)(void*));

t_array_list *array_list_create_with_capacity(unsigned int capacity)
{
    t_array_list *array_list = malloc(sizeof(t_array_list));
    if (!array_list)
        return NULL;
    array_list->element_count = 0;
    array_list->capacity = capacity;
    array_list->array = (void **)malloc(array_current_size(array_list));

    if (!array_list->array)
        return NULL;

    return array_list;
}

t_array_list *array_list_create(void)
{
    return array_list_create_with_capacity(BASE_CAPACITY);
}

void array_list_destroy(t_array_list *self)
{
    free(self->array);
    free(self);
}

void array_list_clean(t_array_list *self)
{
    int len = self->element_count;
    for (int i = 0; i < len; i++)
    {
        self->array[i] = 0;
    }

    self->element_count = 0;
}

void array_list_clean_and_destroy_elements(t_array_list *self, void (*element_destroyer)(void *))
{
    int len = self->element_count;
    for (int i = 0; i < len; i++)
    {
        element_destroyer(self->array[i]);
        self->array[i] = 0;
    }
    self->element_count = 0;
}

void array_list_destroy_and_destroy_elements(t_array_list *self, void (*element_destroyer)(void *))
{
    array_list_clean_and_destroy_elements(self, element_destroyer);
    array_list_destroy(self);
}

void array_list_foreach(t_array_list *self, void (*operation)(void *))
{
    int len = self->element_count;
    for (int i = 0; i < len; i++)
    {
        operation(self->array[i]);
    }
}

t_list_error array_list_add_to_index(t_array_list *self, int index, void *data)
{
    if (index_out_of_bounds(self, index))
        return LIST_INDEX_OUT_OF_BOUNDS;

    if (array_needs_resizing(self))
    {
        resize_array(self);
    }
    shif_elements_to_right(self, index);
    self->array[index] = data;
    self->element_count++;
    return LIST_SUCCESS;
}

void array_list_add(t_array_list *self, void *data)
{
    // handle resizing
    array_list_add_to_index(self, self->element_count, data);
}

t_list_error array_list_get(t_array_list *self, int index, void **out_buffer)
{
    if (index_out_of_bounds(self, index))
        return LIST_INDEX_OUT_OF_BOUNDS;
    if (out_buffer)
    {
        *out_buffer = self->array[index];
    }
    return LIST_SUCCESS;
}

unsigned int array_list_size(t_array_list *self)
{
    return self->element_count;
}

bool array_list_is_empty(t_array_list *self)
{
    return array_list_size(self) == 0;
}

t_list_error array_list_remove(t_array_list *self, int index, void **deleted)
{
    return remove_element(self,index,deleted,NULL);
}

static bool index_out_of_bounds(t_array_list *self, int index)
{
    return index < 0 || index > (int)array_list_size(self);
}

static t_list_error remove_element(t_array_list *self, int index, void **deleted, void(*element_destroyer)(void*))
{

    if (index_out_of_bounds(self, index))
        return LIST_INDEX_OUT_OF_BOUNDS;
    
    if(*deleted)
        *deleted = self->array[index];
    else if(element_destroyer){
        element_destroyer(self->array[index]);
    }
    
    self->array[index] = 0;
    shift_elements_to_left(self, index);
    self->element_count--;
    return LIST_SUCCESS;
}



static void resize_array(t_array_list *self)
{
    self->capacity *= CAPACITY_MULTIPLIER;
    self->array = realloc(self->array, array_current_size(self));
    if (self->array == NULL)
    {
        fprintf(stderr, "Not enough memory for resizing array list %p", (void *)self);
    }
}

static size_t array_current_size(t_array_list *self)
{
    return self->capacity * sizeof(void *);
}

static bool array_needs_resizing(t_array_list *self)
{
    return self->element_count >= self->capacity;
}

static void shif_elements_to_right(t_array_list *self, int start)
{
    memmove(&self->array[start + 1], &self->array[start],
            (self->element_count - start) * sizeof(void *));
}

static void shift_elements_to_left(t_array_list *self, int end)
{
    if ((unsigned int) end < self->element_count - 1)
    {
        memmove(&self->array[end], &self->array[end + 1],
                (self->element_count - end - 1) * sizeof(void *));
    }
}
