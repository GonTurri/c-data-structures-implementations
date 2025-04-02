#include "hashmap.h"

static t_hash_node *create_node(char *key, void *data, unsigned long hash);
static unsigned long hash_djb2(const char *str);
static t_hash_node *find_node(t_hash_map *map, char *key, unsigned long *out_hash, int *out_index, t_hash_node **out_prev);
static double calc_load_factor(t_hash_map *map);
static void resize(t_hash_map *map, int new_capacity);
static void destroy_node(t_hash_node *node, void(*element_destroyer)(void*));
static void* remove_element(t_hash_map* map, char* key);
static void remove_and_destroy_element(t_hash_map* map, char* key,  void(*element_destroyer)(void*));
static void internal_hash_map_clean_and_destroy_elements(t_hash_map* self,void(*element_destroyer)(void*));
static void increment_map_size(t_hash_map* self);
static void decrement_map_size(t_hash_map* self);

t_hash_map *hash_map_create(void)
{
    t_hash_map *map = malloc(sizeof(t_hash_map));
    if (!map)
        return NULL;

    map->capacity = MAP_INITIAL_CAPACITY;
    map->load_factor = 0;
    map->size = 0;
    map->buckets = calloc(map->capacity, sizeof(t_hash_node*));
    map->hash_function = hash_djb2;
    return map;
}

t_hash_map* hash_map_create_with_hash_function(t_hash_function hash_function){
    t_hash_map* map = hash_map_create();
    if(!map) return NULL;
    map->hash_function = hash_function;
    return map;
}

void hash_map_destroy(t_hash_map *self)
{
    hash_map_clean(self);
    free(self->buckets);
    free(self);
}

void hash_map_destroy_and_destroy_elements(t_hash_map *self, void(*element_destroyer)(void*))
{
    hash_map_clean_and_destroy_elements(self,element_destroyer);
    free(self->buckets);
    free(self);
}

int hash_map_size(t_hash_map* self){
    return self->size;
}

void hash_map_put(t_hash_map *self, char *key, void *data)
{
    unsigned long hash;
    int index;
    
    t_hash_node *existing_node = find_node(self, key, &hash, &index,NULL);

    if (existing_node)
    {
        existing_node->value = data;
        return;
    }

    // Key not found, create new node
    t_hash_node *new = create_node(key, data, hash);
    new->next = self->buckets[index];
    self->buckets[index] = new;
    
    increment_map_size(self);

    // Resize if necessary
    if (self->load_factor > DEFAULT_LOAD_FACTOR) {
        resize(self, self->capacity * DEFAULT_CAPACITY_MULTIPLIER);
        self->load_factor = calc_load_factor(self);
    }
}

void* hash_map_get(t_hash_map* self, char* key){
    t_hash_node* node = find_node(self,key,NULL,NULL,NULL);
    return node ? node->value : NULL;
}

void* hash_map_remove(t_hash_map* self, char* key){
    return remove_element(self,key);
}

void hash_map_remove_and_destroy_element(t_hash_map* self, char* key, void(*element_destroyer)(void*)){
     remove_and_destroy_element(self,key,element_destroyer);
}


void hash_map_iterate(t_hash_map* map, void(*iterator)(char*,void*)){
    t_hash_node* node = NULL;
    for(int i =0; i< map->capacity; i++){
        node = map->buckets[i];
        while(node){
            iterator(node->key,node->value);
            node = node->next;
        }
    }
}

void hash_map_clean(t_hash_map* self){
    internal_hash_map_clean_and_destroy_elements(self,NULL);
}

void hash_map_clean_and_destroy_elements(t_hash_map* self, void(*element_destroyer)(void*)){
    internal_hash_map_clean_and_destroy_elements(self,element_destroyer);
}


static double calc_load_factor(t_hash_map *map)
{
    return (double)map->size / (double)map->capacity;
}

static t_hash_node *find_node(t_hash_map *map, char *key, unsigned long *out_hash, int *out_index, t_hash_node **out_prev)
{
    unsigned long hash = map->hash_function(key);
    int index = hash % map->capacity;

    if (out_hash) *out_hash = hash;
    if (out_index) *out_index = index;

    t_hash_node *node = map->buckets[index];
    t_hash_node *prev = NULL;

    while (node)
    {
        if (node->hash == hash && strcmp(node->key, key) == 0) {
            if (out_prev) *out_prev = prev;
            return node;
        }
        prev = node;
        node = node->next;
    }
    return NULL;
}



static void resize(t_hash_map *map, int new_capacity)
{
    t_hash_node** new_buckets = calloc(new_capacity, sizeof(t_hash_node*));
    t_hash_node** old_buckets = map->buckets;

    for (int i = 0; i < map->capacity; i++) {
        t_hash_node* old = old_buckets[i];
        t_hash_node* next;

        while (old) {
            next = old->next; 
            int new_index = old->hash % new_capacity;

            old->next = new_buckets[new_index]; 
            new_buckets[new_index] = old;

            old = next;
        }
    }

    map->buckets = new_buckets;
    map->capacity = new_capacity;
    free(old_buckets);
}


static void* remove_element(t_hash_map* map, char* key){
    int bucket_index;
    t_hash_node* prev = NULL;
    t_hash_node* to_delete = find_node(map,key,NULL,&bucket_index,&prev);
    if(!to_delete) return NULL;

    
    void* data = to_delete->value;

    if (!prev) {
        map->buckets[bucket_index] = to_delete->next;
    }
    else {
        prev->next = to_delete->next; 
    }
    
    decrement_map_size(map);
    destroy_node(to_delete,NULL);
    return data;
}

static void remove_and_destroy_element(t_hash_map* map, char* key,  void(*element_destroyer)(void*)){
    int bucket_index;
    t_hash_node* prev = NULL;
    t_hash_node* to_delete = find_node(map,key,NULL,&bucket_index,&prev);

    if(!to_delete) return;

    if (!prev) {
        map->buckets[bucket_index] = to_delete->next;
    }
    else {
        prev->next = to_delete->next; 
    }
    
    decrement_map_size(map);
    destroy_node(to_delete,element_destroyer);

}

static void internal_hash_map_clean_and_destroy_elements(t_hash_map* self,void(*element_destroyer)(void*)){
    for(int i =0; i< self->capacity; i++){
        t_hash_node* node = self->buckets[i];
        t_hash_node* next = NULL;
        while(node){
            next = node->next;
            destroy_node(node,element_destroyer);
            node = next;
        }
        self->buckets[i] = NULL;
    }
    self->size = 0;
    self->load_factor = 0;
}

static unsigned long hash_djb2(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}

static t_hash_node *create_node(char *key, void *data, unsigned long hash)
{
    t_hash_node *node = malloc(sizeof(t_hash_node));
    if (!node)
        return NULL;

    node->key = strdup(key);
    node->value = data;
    node->next = NULL;
    node->hash = !key ? 0l : hash;
    return node;
}

static void destroy_node(t_hash_node *node, void(*element_destroyer)(void*))
{
    if(element_destroyer) element_destroyer(node->value);
    free(node->key);
    free(node);
}

static void increment_map_size(t_hash_map* self){
    self->size++;
    self->load_factor = calc_load_factor(self);
}

static void decrement_map_size(t_hash_map* self){
    self->size--;
    self->load_factor = calc_load_factor(self);
}

