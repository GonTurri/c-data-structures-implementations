#include <stdio.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../test/collections/list/linked_list_test.h"
#include "../test/collections/queue_stack/queue_stack_test.h"
#include "../test/collections/list/array_list_test.h"




int main(void){
    CU_initialize_registry();
    CU_pSuite linked_list_suite = get_linked_list_suite();
    CU_pSuite stack_and_queue_suite = get_queue_stack_suite();
    CU_pSuite array_list_suite = get_array_list_suite();

    if(NULL  == linked_list_suite || NULL == stack_and_queue_suite
    || NULL == array_list_suite){
        return CU_get_error();
    }
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}

