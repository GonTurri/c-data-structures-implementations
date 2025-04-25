// Node Color: Each node is either red or black.
// Root Property: The root of the tree is always black.
// Red Property: Red nodes cannot have red children (no two consecutive red nodes on any path).
// Black Property: Every path from a node to its descendant null nodes (leaves) has the same number of black nodes.
// Leaf Property: All leaves (NIL nodes) are black.

#include "red_black_tree.h"

static t_rbt_node *create_node(t_key key, void *value);
static void destroy_node(t_rbt_node *node);

static bool find_node(t_rbt_node *root, t_comparator comparator, void *key, t_rbt_node **out, t_rbt_node **prev);
static t_rbt_node *insert_child(t_rb_tree *tree, t_rbt_node *parent, t_key key, void *value);
static void fix_insert(t_rb_tree *tree, t_rbt_node *node);
static void rb_tree_inner_clear_and_destroy_elements(t_rbt_node **node, void (*element_destroyer)(void *));
static t_rbt_node *rb_tree_delete_node(t_rb_tree *tree, void *key, t_rbt_node **out_replacer);
static void fix_deletion(t_rb_tree *tree, t_rbt_node *node, t_rbt_node *parent);
static t_rbt_node *rb_tree_delete_and_fix(t_rb_tree *tree, void *key);
static void rb_tree_inner_iterate_preorder(t_rbt_node *root, void (*iterator)(void *, void *));

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

    t_rbt_node *inserted = insert_child(tree, parent, key, value);
    if (!inserted)
        return false;
    tree->size++;

    if (!inserted->parent || inserted->parent->color == BLACK)
        return true;

    fix_insert(tree, inserted);

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

bool rb_tree_remove(t_rb_tree *tree, void *key, void **out)
{
    t_rbt_node *node = rb_tree_delete_and_fix(tree, key);
    if (!node)
        return false;

    if (out)
    {
        *out = node->value;
    }

    destroy_node(node);

    return true;
}

bool rb_tree_remove_and_destroy(t_rb_tree *tree, void *key, void (*element_destroyer)(void *))
{
    t_rbt_node *node = rb_tree_delete_and_fix(tree, key);
    if (!node)
        return false;

    if (element_destroyer)
    {
        element_destroyer(node->value);
    }

    destroy_node(node);

    return true;
}

static bool find_node(t_rbt_node *root, t_comparator comparator, void *key, t_rbt_node **out, t_rbt_node **parent)
{
    t_rbt_node *current = root;
    t_rbt_node *prev = NULL;

    while (current)
    {
        if (!comparator(key, current->key.data) && !comparator(current->key.data, key))
        {
            if (out)
                *out = current;
            if (parent)
                *parent = prev;
            return true;
        }

        prev = current;
        if (comparator(key, current->key.data))
            current = current->left;
        else
            current = current->right;
    }
    if (parent)
        *parent = prev;
    return false;
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

static t_rbt_node *insert_child(t_rb_tree *tree, t_rbt_node *parent, t_key key, void *value)
{
    t_rbt_node *child = create_node(key, value);
    if (!child)
        return NULL;

    if (rb_tree_is_empty(tree))
    {
        tree->root = child;
        child->color = BLACK;
        return child;
    }

    child->parent = parent;
    if (tree->comparator(key.data, parent->key.data))
    {
        parent->left = child;
    }
    else
    {
        parent->right = child;
    }
    return child;
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
    tree->root->color = BLACK;
}

static t_rbt_node *rb_tree_delete_node(t_rb_tree *tree, void *key, t_rbt_node **out_replacer)
{
    t_rbt_node *node;
    t_rbt_node *parent;
    bool found = find_node(tree->root, tree->comparator, key, &node, &parent);
    if (!found)
        return NULL;

    // case 1: node at most one child
    if (!node->left || !node->right)
    {
        t_rbt_node *child = node->left ? node->left : node->right;
        if (parent)
        {
            if (parent->right == node)
                parent->right = child;
            else
                parent->left = child;
        }
        else
        {
            tree->root = child;
        }
        if (child)
            child->parent = parent;
        if (out_replacer)
            *out_replacer = child;
        return node;
    }
    // node has two children
    else
    {
        // Replace nodes key and content with its in-order successor

        t_rbt_node *aux_parent = NULL;
        t_rbt_node *r_sub_tree = node->right;
        while (r_sub_tree->left)
        {
            aux_parent = r_sub_tree;
            r_sub_tree = r_sub_tree->left;
        }
        if (aux_parent)
            aux_parent->left = r_sub_tree->right;
        else
            node->right = r_sub_tree->right;

        if (r_sub_tree->right)
        {
            r_sub_tree->right->parent = aux_parent ? aux_parent : node;
        }
        free(node->key.data);
        node->key.size = r_sub_tree->key.size;
        node->key.data = malloc(r_sub_tree->key.size);
        memcpy(node->key.data, r_sub_tree->key.data, r_sub_tree->key.size);
        node->value = r_sub_tree->value;
        if (out_replacer)
            *out_replacer = NULL;
        return r_sub_tree;
    }
}

static void fix_deletion(t_rb_tree *tree, t_rbt_node *node, t_rbt_node *parent)
{
    while ((node == NULL || node->color == BLACK) && node != tree->root)
    {
        if (parent->left == node)
        {
            t_rbt_node *sibling = parent->right;

            if (sibling && sibling->color == RED)
            {
                sibling->color = BLACK;
                parent->color = RED;
                left_rotation(tree, parent);
                sibling = parent->right;
            }

            if ((!sibling->left || sibling->left->color == BLACK) &&
                (!sibling->right || sibling->right->color == BLACK))
            {
                if (sibling)
                    sibling->color = RED;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (!sibling->right || sibling->right->color == BLACK)
                {
                    if (sibling->left)
                        sibling->left->color = BLACK;
                    sibling->color = RED;
                    right_rotation(tree, sibling);
                    sibling = parent->right;
                }

                sibling->color = parent->color;
                parent->color = BLACK;
                if (sibling->right)
                    sibling->right->color = BLACK;
                left_rotation(tree, parent);
                node = tree->root;
            }
        }
        else
        {
            t_rbt_node *sibling = parent->left;

            if (sibling && sibling->color == RED)
            {
                sibling->color = BLACK;
                parent->color = RED;
                right_rotation(tree, parent);
                sibling = parent->left;
            }

            if ((!sibling->left || sibling->left->color == BLACK) &&
                (!sibling->right || sibling->right->color == BLACK))
            {
                if (sibling)
                    sibling->color = RED;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (!sibling->left || sibling->left->color == BLACK)
                {
                    if (sibling->right)
                        sibling->right->color = BLACK;
                    sibling->color = RED;
                    left_rotation(tree, sibling);
                    sibling = parent->left;
                }

                sibling->color = parent->color;
                parent->color = BLACK;
                if (sibling->left)
                    sibling->left->color = BLACK;
                right_rotation(tree, parent);
                node = tree->root;
            }
        }
    }

    if (node)
        node->color = BLACK;
}

static t_rbt_node *rb_tree_delete_and_fix(t_rb_tree *tree, void *key)
{
    t_rbt_node *replacer = NULL;
    t_rbt_node *to_delete = rb_tree_delete_node(tree, key, &replacer);

    if (!to_delete)
        return NULL;

    
    if (to_delete->color == BLACK)
    {
        t_rbt_node *fix_parent = replacer ? replacer->parent : to_delete->parent;
        fix_deletion(tree, replacer, fix_parent);
    }

    tree->size--;

    return to_delete;
}

void rb_tree_iterate_preorder(t_rb_tree *tree, void (*iterator)(void *key, void *value))
{
    if (rb_tree_is_empty(tree))
        return;
    rb_tree_inner_iterate_preorder(tree->root, iterator);
}

static void rb_tree_inner_iterate_preorder(t_rbt_node *root, void (*iterator)(void *, void *))
{
    if (!root)
        return;
    iterator(root->key.data, root->value);
    rb_tree_inner_iterate_preorder(root->left, iterator);
    rb_tree_inner_iterate_preorder(root->right, iterator);
}
