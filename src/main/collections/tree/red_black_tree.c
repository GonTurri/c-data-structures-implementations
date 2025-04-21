// Node Color: Each node is either red or black.
// Root Property: The root of the tree is always black.
// Red Property: Red nodes cannot have red children (no two consecutive red nodes on any path).
// Black Property: Every path from a node to its descendant null nodes (leaves) has the same number of black nodes.
// Leaf Property: All leaves (NIL nodes) are black.

#include "red_black_tree.h"

static t_rbt_node *create_node(t_key key, void *value);
static void destroy_node(t_rbt_node *node);

static bool find_node(t_rbt_node *root, t_comparator comparator, void *key, t_rbt_node **out, t_rbt_node **prev);
static bool insert_child(t_rb_tree *tree, t_rbt_node *parent, t_key key, void *value);
static void fix_insert(t_rb_tree *tree, t_rbt_node *node);
static void rb_tree_inner_clear_and_destroy_elements(t_rbt_node **node, void (*element_destroyer)(void *));

t_rb_tree *rbt_tree_create(t_comparator comparator)
{
    t_rb_tree *tree = malloc(sizeof(t_rb_tree));
    if (!tree || !comparator)
        return NULL;
    tree->comparator = comparator;
    tree->size = 0;
    tree->root = NULL;
    return tree;
}

void rb_tree_destroy(t_rb_tree *tree)
{
    rb_tree_clear(tree);
    free(tree);
}

void rb_tree_destroy_and_destroy_elements(t_rb_tree *tree, void (*element_destroyer)(void *))
{
    rb_tree_clear_and_destroy_elements(tree, element_destroyer);
    free(tree);
}

void rb_tree_clear(t_rb_tree *tree)
{
    rb_tree_clear_and_destroy_elements(tree, NULL);
}

void rb_tree_clear_and_destroy_elements(t_rb_tree *tree, void (*element_destroyer)(void *))
{
    rb_tree_inner_clear_and_destroy_elements(&tree->root, element_destroyer);
    tree->size = 0;
}

static void rb_tree_inner_clear_and_destroy_elements(t_rbt_node **node, void (*element_destroyer)(void *))
{
    if (!node || !*node)
        return;

    rb_tree_inner_clear_and_destroy_elements(&((*node)->left), element_destroyer);
    rb_tree_inner_clear_and_destroy_elements(&((*node)->right), element_destroyer);

    if (element_destroyer)
    {
        element_destroyer((*node)->value);
    }

    destroy_node(*node);
    *node = NULL;
}

bool rb_tree_insert(t_rb_tree *tree, t_key key, void *value)
{
    t_rbt_node *aux, *parent;
    bool res = find_node(tree->root, tree->comparator, key.data, &aux, &parent);
    if (res)
    {
        aux->value = value;
        return res;
    }

    res = insert_child(tree, parent, key, value);
    if (!res)
        return res;
    tree->size++;

    if (!parent || parent->color == BLACK)
        return true;

    fix_insert(tree, aux);

    return true;
}

bool rb_tree_find(t_rb_tree *tree, void *key, void **out)
{
    t_rbt_node *aux;
    bool res = find_node(tree->root, tree->comparator, key, &aux, NULL);
    if (res)
    {
        if (out)
            *out = aux->value;
    }
    return res;
}

static bool find_node(t_rbt_node *root, t_comparator comparator, void *key, t_rbt_node **out, t_rbt_node **parent)
{
    if (!root)
        return false;

    if (parent)
        *parent = root->parent;

    if (!comparator(key, root->key.data) && !comparator(root->key.data, key))
    {
        if (out)
            *out = root;
        return true;
    }

    if (comparator(key, root->key.data))
    {
        return find_node(root->left, comparator, key, out, parent);
    }

    return find_node(root->right, comparator, key, out, parent);
}

int rb_tree_size(t_rb_tree *tree)
{
    return tree->size;
}

bool rb_tree_is_empty(t_rb_tree *tree)
{
    return rb_tree_size(tree) == 0;
}

static t_rbt_node *create_node(t_key key, void *value)
{
    t_rbt_node *node = malloc(sizeof(t_rbt_node));
    if (!node)
        return NULL;
    node->key.data = malloc(key.size);
    if (!node->key.data)
    {
        free(node);
        return NULL;
    }
    node->key.size = key.size;
    memcpy(node->key.data, key.data, key.size);
    node->color = RED;
    node->value = value;
    node->right = node->parent = node->left = NULL;
    return node;
}

static void destroy_node(t_rbt_node *node)
{
    if (!node)
        return;
    free(node->key.data);
    free(node);
}

static void right_rotation(t_rb_tree *tree, t_rbt_node *x)
{
    t_rbt_node *y = x->left;
    x->left = y->right;
    y->parent = x->parent;
    if (y->right)
    {
        y->right->parent = x;
    }
    if (!x->parent && tree->root == x)
    {
        tree->root = y;
    }
    else if (x->parent->right && x->parent->right == x)
    {
        x->parent->right = y;
    }
    else
    {
        x->parent->left = y;
    }
    x->parent = y;
    y->right = x;
    tree->root->color = BLACK;
}

static void left_rotation(t_rb_tree *tree, t_rbt_node *x)
{
    t_rbt_node *y = x->right;
    x->right = y->left;
    y->parent = x->parent;
    if (y->left)
    {
        y->left->parent = x;
    }
    if (!x->parent && tree->root == x)
    {
        tree->root = y;
    }
    else if (x->parent->right && x->parent->right == x)
    {
        x->parent->right = y;
    }
    else
    {
        x->parent->left = y;
    }
    y->left = x;
    x->parent = y;
    tree->root->color = BLACK;
}

static bool insert_child(t_rb_tree *tree, t_rbt_node *parent, t_key key, void *value)
{
    t_rbt_node *child = create_node(key, value);
    if (!child)
        return false;
    child->parent = parent;
    if (rb_tree_is_empty(tree))
    {
        tree->root = child;
        child->color = BLACK;
    }
    else if (tree->comparator(key.data, parent->key.data))
    {
        parent->left = child;
    }
    else
    {
        parent->right = child;
    }
    return true;
}

static void fix_insert(t_rb_tree *tree, t_rbt_node *node)
{
    while ((node != tree->root) && (RED == node->parent->color))
    {
        t_rbt_node *parent = node->parent;
        t_rbt_node *grand_parent = parent->parent;
        if (parent == grand_parent->left)
        {
            t_rbt_node *uncle = grand_parent->right;

            if (uncle && (RED == uncle->color))
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grand_parent->color = RED;
                node = grand_parent;
            }
            else
            {
                // right-right
                if (node == parent->right)
                {
                    node = parent;
                    left_rotation(tree, node);
                }
                // right - left
                parent = node->parent;
                grand_parent = parent->parent;
                parent->color = BLACK;
                grand_parent->color = RED;
                right_rotation(tree, grand_parent);
            }
        }
        else
        {
            // mirror
            t_rbt_node *uncle = grand_parent->left;
            if (uncle && (RED == uncle->color))
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                grand_parent->color = RED;
                node = grand_parent;
            }
            else
            {
                // left-left
                if (node == parent->left)
                {
                    node = parent;
                    right_rotation(tree, node);
                }
                // left -right
                parent = node->parent;
                grand_parent = parent->parent;
                parent->color = BLACK;
                grand_parent->color = RED;
                left_rotation(tree, grand_parent);
            }
        }
    }
}
