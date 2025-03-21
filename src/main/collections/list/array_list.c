#include "array_list.h"

static bool index_out_of_bounds(t_array_list* self, int index);

t_array_list* array_list_create_with_capacity(size_t element_size,unsigned int capacity){
    t_array_list* array_list = malloc(sizeof(t_array_list));
    array_list->element_count = 0;
    array_list->capacity = capacity;
    array_list->element_size = element_size;
    array_list->array = (void**) malloc(capacity*element_size*sizeof(void*));
    return  array_list;
}


t_array_list* array_list_create(size_t element_size){
    return array_list_create_with_capacity(element_size,BASE_CAPACITY);
}

void array_list_destroy(t_array_list* self){
    free(self->array);
    free(self);
    self = NULL;
}



void array_list_clean(t_array_list* self){
    int len = self->element_count;
    for(int i = 0; i<len; i++){
        self->array[i] = 0;
    }

    self->element_count = 0;
}

void array_list_clean_and_destroy_elements(t_array_list* self,void(*element_destroyer)(void*)){
    int len = self->element_count;
    for(int i = 0; i<len; i++){
        element_destroyer(self->array[i]);
        self->array[i] = 0;
    }
    self->element_count = 0;
}


void array_list_destroy_and_destroy_elements(t_array_list* self,void(*element_destroyer)(void*)){
    array_list_clean_and_destroy_elements(self,element_destroyer);
    array_list_destroy(self);
}

void array_list_foreach(t_array_list* self, void(*operation)(void*)){
    int len = self->element_count;
    for(int i =0; i< len; i++){
        operation(self->array[i]);
    }
} 


void array_list_add(t_array_list* self, void* data){
    // handle resizing
    self->array[self->element_count] = data;
    self->element_count++;
}

t_list_error array_list_get(t_array_list* self, int index, void** out_buffer){
    if(index_out_of_bounds(self,index)) return LIST_INDEX_OUT_OF_BOUNDS;
    if(out_buffer){
        *out_buffer = self->array[index];
    }
    return LIST_SUCCESS;
}

unsigned int array_list_size(t_array_list* self){
    return self->element_count;
}

bool array_list_is_empty(t_array_list* self){
    return array_list_size(self) == 0;
}

static bool index_out_of_bounds(t_array_list* self, int index){
    return index < 0 || index >= (int) array_list_size(self);
}

// static void remove_element(t_array_list* self, int index){
// }

