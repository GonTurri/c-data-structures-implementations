#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

typedef struct double_l_node{
    void* data;
    struct double_l_node* next;
    struct double_l_node* prev;
} t_double_l_node;

typedef struct hash_element{
    unsigned long hash;
    char* key;
    void* value;
    struct hash_element* next;
} t_hash_node;



#endif

