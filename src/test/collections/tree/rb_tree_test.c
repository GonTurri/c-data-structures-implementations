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
    CU_ASSERT_EQUAL(rb_tree_size(tree),1);
    CU_ASSERT_FALSE(res2);
}

CU_pSuite get_rb_tree_suite(void)
{
    CU_pSuite suite = CU_add_suite("Red black tree suite", init_suite, clean_suite);
    CU_add_test(suite, "red black tree, test of insert", test_rb_tree_insert);

    return suite;
}
