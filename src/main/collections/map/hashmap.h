#ifndef HASH_MAP_H_INCLUDED
#define HASH_MAP_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include "../node.h"

#define MAP_INITIAL_CAPACITY 10
#define DEFAULT_LOAD_FACTOR 0.7
#define DEFAULT_CAPACITY_MULTIPLIER 2


typedef struct{
    int size;
    int capacity;
    double load_factor;
    t_hash_node** buckets;
} t_hash_map;


t_hash_map* hash_map_create(void);

void hash_map_destroy(t_hash_map *self);

void hash_map_destroy_and_destroy_elements(t_hash_map *self, void(*element_destroyer)(void*));

void hash_map_clean(t_hash_map* self);

void hash_map_clean_and_destroy_elements(t_hash_map* self, void(*element_destroyer)(void*));

void hash_map_put(t_hash_map *self, char *key, void *data);

void* hash_map_get(t_hash_map* self, char* key);

void* hash_map_remove(t_hash_map* self, char* key);

void hash_map_remove_and_destroy_element(t_hash_map* self, char* key, void(*element_destroyer)(void*));

int hash_map_size(t_hash_map* self);

#endif

