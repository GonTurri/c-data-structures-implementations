#include "linked_list_test.h"

static t_linked_list* list;

static void add_random_elements(t_linked_list* list){
    int* a = malloc(sizeof(int));
    *a = 5;

    int* b = malloc(sizeof(int));
    *b = 3;

    int* c = malloc(sizeof(int));
    *c = 1;

    
    linked_list_add(list,a);
    linked_list_add(list,b);
    linked_list_add(list,c);
}

static void remove_random_elements(t_linked_list* list){
    linked_list_clean_and_destroy_elements(list,free);
}

// should add the elements 5,3,1 to the list

static void test_linked_list_add(void)
{
    int* buf;

    add_random_elements(list);

    CU_ASSERT(linked_list_get(list,0,(void**) &buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf,5);
    CU_ASSERT(linked_list_get(list,1,(void**) &buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf,3);
    CU_ASSERT(linked_list_get(list,2,(void**) &buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf,1);
    CU_ASSERT_EQUAL(linked_list_size(list),3);
    remove_random_elements(list);

}

static void test_linked_list_add_first(void){

    int* buf;

    int* x = malloc(sizeof(int));
    *x = 10;

    add_random_elements(list);

    linked_list_add_first(list,x);

    linked_list_get(list,0,(void**) &buf);
    CU_ASSERT_EQUAL(*buf,10);

    remove_random_elements(list);
}


static bool find_condition_success(void* elem){
    return *((int*) elem) < 2;
}

static bool find_condition_failure(void* elem){
    return *((int*) elem) < 0;
}

static  void test_linked_list_find(void){
    int *buf;
    add_random_elements(list);
    t_list_error err = linked_list_find(list,find_condition_success, (void**) &buf);
    CU_ASSERT_EQUAL(err,LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf,1);
    remove_random_elements(list);
}

static  void test_linked_list_find_not_found(void){
    int *buf;
    add_random_elements(list);
    t_list_error err = linked_list_find(list,find_condition_failure, (void**) &buf);
    CU_ASSERT_EQUAL(err,LIST_NOT_FOUND);
    remove_random_elements(list);
}

// should return LIST_INDEX_OUT_OF_BOUNDS when trying to get and out of bounds position

static void test_linked_list_get_errors(void){
    int* buf;

    add_random_elements(list);

    t_list_error err = linked_list_get(list,-1, (void**) &buf);
    CU_ASSERT_EQUAL(err,LIST_INDEX_OUT_OF_BOUNDS);
    err = linked_list_get(list,3,(void**) &buf);
    CU_ASSERT_EQUAL(err,LIST_INDEX_OUT_OF_BOUNDS);

    remove_random_elements(list);

}

static void test_linked_list_remove_by_index(void){
    int *buf;
    add_random_elements(list);
    
    linked_list_remove(list,1,(void**) &buf);
    CU_ASSERT_EQUAL(*buf,3);
    CU_ASSERT_EQUAL(linked_list_size(list),2);
    linked_list_remove(list,0,(void**) &buf);
    CU_ASSERT_EQUAL(*buf,5);
    CU_ASSERT_EQUAL(linked_list_size(list),1);
    linked_list_remove(list,0,(void**) &buf);
    CU_ASSERT_EQUAL(*buf,1);
    CU_ASSERT(linked_list_is_empty(list));
    

}

static void test_linked_list_clean(void){
    int x =5, y=10;
    linked_list_add(list,&x);
    linked_list_add(list,&y);

    linked_list_clean(list);
    
    CU_ASSERT(linked_list_is_empty(list));
    CU_ASSERT_EQUAL(x,5);
    CU_ASSERT_EQUAL(y,10);

}

static void plus_one(void * elem){
    int* num = (int*) elem;
    (*num)++;
}

static void test_linked_list_foreach(void){
    add_random_elements(list);
    linked_list_foreach(list,plus_one);

    int * buf;

    CU_ASSERT(linked_list_get(list,0,(void**) &buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf,6);
    CU_ASSERT(linked_list_get(list,1,(void**) &buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf,4);
    CU_ASSERT(linked_list_get(list,2,(void**) &buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf,2);

    remove_random_elements(list);
}

static void test_linked_list_set(void){
    int* buf;
    int* x = malloc(sizeof(int));
    *x = 10;
    add_random_elements(list);
    linked_list_set(list,0, x, (void**) &buf);
    CU_ASSERT_EQUAL(*buf,5);
    CU_ASSERT(linked_list_get(list,0,(void**) &buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf,10);


    remove_random_elements(list);
}

static void test_linked_list_replace_by_condition(void){
    int* buf;
    int* x = malloc(sizeof(int));
    *x = 10;
    add_random_elements(list);
    linked_list_replace_by_condition(list,find_condition_success, x, (void**) &buf);
    CU_ASSERT_EQUAL(*buf,1);
    CU_ASSERT(linked_list_find(list,find_condition_success,(void**) &buf) == LIST_NOT_FOUND);
    remove_random_elements(list);
}


static int init_linked_list(void){
    list = linked_list_create();
    return 0;
}

static int clean_linked_list(void){
    linked_list_clean_and_destroy_elements(list,free);
    return 0;
}
CU_pSuite get_linked_list_suite(void){
    CU_pSuite suite = CU_add_suite("Linked list suite", init_linked_list, NULL);
    CU_add_test(suite, "test of linked_list_add()", test_linked_list_add);
    CU_add_test(suite, "test of linked_list_add_first()", test_linked_list_add_first);
    CU_add_test(suite, "test of linked_list_get() when index out of bounds", test_linked_list_get_errors);
    CU_add_test(suite, "test of linked_list_set()", test_linked_list_set);
    CU_add_test(suite, "test of linked_list_replace_by_condition()", test_linked_list_replace_by_condition);
    CU_add_test(suite, "test of linked_list_find() when found", test_linked_list_find);
    CU_add_test(suite, "test of linked_list_find() when not found", test_linked_list_find_not_found);
    CU_add_test(suite,"test of linked_list_remove()",test_linked_list_remove_by_index);
    CU_add_test(suite,"test of linked_list_clean()",test_linked_list_clean);  
    CU_add_test(suite,"test of linked_list_foreach()",test_linked_list_foreach);
    return suite;
}

