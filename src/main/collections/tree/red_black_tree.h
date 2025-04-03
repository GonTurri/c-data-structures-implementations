#ifndef RED_BLACK_TREE_H_INCLUDED
#define RED_BLACK_TREE_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>

#define RED false
#define BLACK true

typedef bool node_color;


//same type as the key
typedef bool (*t_comparator)(void *, void *);

typedef struct node
{
    void* key;
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


t_rb_tree* rbt_tree_create(t_comparator comparator);

void rb_tree_destroy(t_rb_tree* tree);

#endif
