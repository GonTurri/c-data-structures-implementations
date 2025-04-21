#ifndef RED_BLACK_TREE_H_INCLUDED
#define RED_BLACK_TREE_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define RED false
#define BLACK true

typedef bool node_color;

// same type as the key
typedef bool (*t_comparator)(void *, void *);

typedef struct
{
    void *data;
    size_t size;
} t_key;

typedef struct node
{
    t_key key;
    void *value;
    node_color color;
    struct node *left;
    struct node *right;
    struct node *parent;
} t_rbt_node;

typedef struct
{
    int size;
    t_rbt_node *root;
    t_comparator comparator;
} t_rb_tree;

t_rb_tree *rbt_tree_create(t_comparator comparator);

int rb_tree_size(t_rb_tree *tree);

bool rb_tree_is_empty(t_rb_tree *tree);

bool rb_tree_find(t_rb_tree *tree, void *key, void **out);

bool rb_tree_insert(t_rb_tree *tree, t_key key, void *value);

void rb_tree_destroy(t_rb_tree *tree);

void rb_tree_destroy_and_destroy_elements(t_rb_tree *tree, void (*element_destroyer)(void *));

void rb_tree_clear(t_rb_tree *tree);

void rb_tree_clear_and_destroy_elements(t_rb_tree *tree, void (*element_destroyer)(void *));

#endif
