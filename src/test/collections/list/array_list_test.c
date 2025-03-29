#include "array_list_test.h"

static t_array_list *list;

static int init_suite(void)
{
    list = array_list_create();
    return 0;
}

static int clean_suite(void)
{
    array_list_destroy(list);
    return 0;
}

static void add_random_ints(void)
{
    int *x = malloc(sizeof(int));
    int *y = malloc(sizeof(int));

    *x = 10;
    *y = 5;

    array_list_add(list, x);
    array_list_add(list, y);
}

static void remove_random_ints(void)
{
    array_list_clean_and_destroy_elements(list, free);
}

static void test_array_list_add_no_resizing(void)
{
    array_list_add(list, &(int){1});
    array_list_add(list, &(int){2});

    int *buf1, *buf2;

    t_list_error err1 = array_list_get(list, 0, (void **)&buf1);
    t_list_error err2 = array_list_get(list, 1, (void **)&buf2);

    CU_ASSERT_EQUAL(err1, LIST_SUCCESS);
    CU_ASSERT_EQUAL(err2, LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf1, 1);
    CU_ASSERT_EQUAL(*buf2, 2);
    CU_ASSERT_EQUAL(array_list_size(list), 2);
    array_list_clean(list);
}

static void test_array_list_add_to_index(void)
{
    add_random_ints();
    int *i = malloc(sizeof(int));
    *i = 8;
    array_list_add_to_index(list, 1, i);
    int *buf1, *buf2, *buf3;

    array_list_get(list, 0, (void **)&buf1);
    array_list_get(list, 1, (void **)&buf2);
    array_list_get(list, 2, (void **)&buf3);
    CU_ASSERT_EQUAL(*buf1, 10);
    CU_ASSERT_EQUAL(*buf2, 8);
    CU_ASSERT_EQUAL(*buf3, 5);
    CU_ASSERT_EQUAL(array_list_size(list), 3);
    remove_random_ints();
}

static void test_array_list_remove(void){
    add_random_ints();
    add_random_ints();
    int* deleted;
    int *buf1, *buf2,*buf3;
    array_list_remove(list,1,(void**) &deleted);
    array_list_get(list, 0, (void **)&buf1);
    array_list_get(list, 1, (void **)&buf2);
    array_list_get(list, 2, (void **)&buf3);

    CU_ASSERT_EQUAL(array_list_size(list),3);
    CU_ASSERT_EQUAL(*deleted,5);
    CU_ASSERT_EQUAL(*buf1,10);
    CU_ASSERT_EQUAL(*buf2,10);
    CU_ASSERT_EQUAL(*buf3,5);


    free(deleted);
    remove_random_ints();
}

static void test_array_list_resizing(void)
{
    t_array_list *arr = array_list_create_with_capacity(2);
    array_list_add(arr, &(int){2});
    array_list_add(arr, &(int){7});
    array_list_add_to_index(arr, 0, &(int){1});

    int *buf1, *buf2, *buf3;
    array_list_get(arr, 0, (void **)&buf1);
    array_list_get(arr, 1, (void **)&buf2);
    array_list_get(arr, 2, (void **)&buf3);
    array_list_get(list, 2, (void **)&buf3);
    CU_ASSERT_EQUAL(array_list_size(arr), 3);
    CU_ASSERT_EQUAL(*buf1, 1);
    CU_ASSERT_EQUAL(*buf2, 2);
    CU_ASSERT_EQUAL(*buf3, 7);
    CU_ASSERT(arr->capacity > 2);
    array_list_destroy(arr);
}

CU_pSuite get_array_list_suite(void)
{
    CU_pSuite suite = CU_add_suite("Array list suite", init_suite, clean_suite);
    CU_add_test(suite, "Test of array list add with no resizing", test_array_list_add_no_resizing);
    CU_add_test(suite, "Test of array list add to and index with no resizing", test_array_list_add_to_index);
    CU_add_test(suite, "Test of array list add to and index with resizing", test_array_list_resizing);
    CU_add_test(suite, "Test of array list remove", test_array_list_remove);
    return suite;
}
