#include "red_black_tree.h"

static t_rbt_node* create_node(void* key, void* value);

static bool find_node(t_rbt_node* root,t_comparator comparator, void* key, t_rbt_node** out);

t_rb_tree* rbt_tree_create(t_comparator comparator){
    t_rb_tree* tree = malloc(sizeof(t_rb_tree));
    if(!tree || !comparator) return NULL;
    tree->comparator = comparator;
    tree->size =0;
    tree->root = NULL;
    return tree;
}

void rb_tree_destroy(t_rb_tree* tree){
    free(tree);
}


bool rb_tree_find(t_rb_tree* tree, void* key,void** out){
    t_rbt_node* aux;
    bool res = find_node(tree->root,tree->comparator,key,&aux);
    if(res){
        if(out) *out = aux->value;
    }
    return res;
}

static bool find_node(t_rbt_node* root,t_comparator comparator, void* key, t_rbt_node** out){
    if(!root) return false;

    if(!comparator(key,root->key) && !comparator(root->key,key)){
        if(out) *out = root;
        return true;
    }

    if(comparator(key,root->key)){
       return find_node(root->left,comparator,key,out);
    }

    return find_node(root->right,comparator,key,out);

}

static t_rbt_node* create_node(void* key,void* value){
    t_rbt_node* node = malloc(sizeof(t_rbt_node));
    if(!node) return NULL;
    node->key = key;
    node->color = RED;
    node->value = value;
    node->right = node->parent = node->left = NULL;
    return node;
}

