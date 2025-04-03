#include "red_black_tree.h"

static t_rbt_node* create_node(void* value);

t_rb_tree* rbt_tree_create(t_comparator comparator){
    t_rb_tree* tree = malloc(sizeof(t_rb_tree));
    if(!tree || !comparator) return NULL;
    tree->comparator = comparator;
    tree->size =0;
    tree->root = NULL;
}

void rb_tree_destroy(t_rb_tree* tree){
    free(tree);
}

static t_rbt_node* create_node(void* value){
    t_rbt_node* node = malloc(sizeof(t_rbt_node));
    if(!node) return NULL;
    node->color = RED;
    node->value = value;
    node->right = node->parent = node->left = NULL;
    return node;
}

