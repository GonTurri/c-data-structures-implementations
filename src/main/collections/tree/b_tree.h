#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define M 5

typedef struct btree{
    uint32_t keys[M-1];
    uint32_t num_of_keys;
    struct btree* children[M];
    void* data;
    bool is_leaf;
} t_btree;

#endif

