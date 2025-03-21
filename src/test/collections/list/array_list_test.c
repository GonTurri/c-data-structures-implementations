#include "array_list_test.h"

static t_array_list* list;

static int init_suite(void){
    list = array_list_create(sizeof(int));
    return 0;
}

static int clean_suite(void){
    array_list_destroy(list);
    return 0;
}

static void test_array_list_add_no_resizing(void){
    array_list_add(list,&(int){1});
    array_list_add(list,&(int){2});

    int* buf1, *buf2;

    t_list_error err1 = array_list_get(list,0,(void**) &buf1);
    t_list_error err2 = array_list_get(list,1, (void**) &buf2);

    CU_ASSERT_EQUAL(err1,LIST_SUCCESS);
    CU_ASSERT_EQUAL(err2,LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf1,1);
    CU_ASSERT_EQUAL(*buf2,2);
    CU_ASSERT_EQUAL(array_list_size(list),2);
    array_list_clean(list);
}

CU_pSuite get_array_list_suite(void){
    CU_pSuite suite = CU_add_suite("Array list suite",init_suite,clean_suite);
    CU_ADD_TEST(suite,test_array_list_add_no_resizing);
    return suite;
}

