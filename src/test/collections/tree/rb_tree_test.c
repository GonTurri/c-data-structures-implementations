#include "rb_tree_test.h"

static t_rb_tree *tree;

static bool comparator(void *n1, void *n2)
{
    return *((int *)n1) < *((int *)n2);
}

static int init_suite(void)
{
    tree = rbt_tree_create(comparator);
    return 0;
}

static int clean_suite(void)
{
    rb_tree_destroy(tree);
    return 0;
}

static void test_rb_tree_insert(void)
{
    int *key = malloc(sizeof(int));
    *key = 5;
    rb_tree_insert(tree, (t_key){.size = sizeof(int), .data = key}, "jorge");

    void *buf;
    bool res = rb_tree_find(tree, &(int){5}, &buf);
    bool res2 = rb_tree_find(tree, &(int){1}, NULL);
    CU_ASSERT(res);
    CU_ASSERT_STRING_EQUAL("jorge", (char *)buf);
    CU_ASSERT_EQUAL(rb_tree_size(tree), 1);
    CU_ASSERT_FALSE(res2);
    rb_tree_clear(tree);
    CU_ASSERT_TRUE(rb_tree_is_empty(tree));
    free(key);
}

static void print_key(void *key, void *value)
{
    printf("%d ; ", *(int *)key);
}

static void test_rb_tree_delete(void)
{
    rb_tree_insert(tree, (t_key){.size = sizeof(int), .data = &(int){4}}, "a");
    rb_tree_insert(tree, (t_key){.size = sizeof(int), .data = &(int){2}}, "a");
    rb_tree_insert(tree, (t_key){.size = sizeof(int), .data = &(int){6}}, "a");
    rb_tree_insert(tree, (t_key){.size = sizeof(int), .data = &(int){1}}, "a");
    rb_tree_insert(tree, (t_key){.size = sizeof(int), .data = &(int){3}}, "a");
    rb_tree_insert(tree, (t_key){.size = sizeof(int), .data = &(int){5}}, "a");
    rb_tree_insert(tree, (t_key){.size = sizeof(int), .data = &(int){8}}, "a");
    rb_tree_insert(tree, (t_key){.size = sizeof(int), .data = &(int){7}}, "a");
    rb_tree_insert(tree, (t_key){.size = sizeof(int), .data = &(int){9}}, "a");

    CU_ASSERT_EQUAL(rb_tree_size(tree), 9);

    rb_tree_iterate_preorder(tree, print_key);
    printf("\n");

    char *temp;

    bool deleted_success = rb_tree_remove(tree, &(int){6}, (void **)&temp);

    CU_ASSERT_TRUE(deleted_success);

    CU_ASSERT_EQUAL(rb_tree_size(tree), 8);

    CU_ASSERT_STRING_EQUAL(temp, "a");

    // should be 4 ; 2 ; 1 ; 3 ; 7 ; 5 ; 8 ; 9

    rb_tree_iterate_preorder(tree, print_key);
    printf("\n");
    rb_tree_clear(tree);
}

CU_pSuite get_rb_tree_suite(void)
{
    CU_pSuite suite = CU_add_suite("Red black tree suite", init_suite, clean_suite);
    CU_add_test(suite, "red black tree, test of insert", test_rb_tree_insert);
    CU_add_test(suite, "red black tree, test of delete", test_rb_tree_delete);

    return suite;
}
