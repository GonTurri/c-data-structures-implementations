#include "hash_map_test.h"

static t_hash_map* map;

static void test_hash_map_put(void){
    hash_map_put(map,"1",strdup("uno"));
    hash_map_put(map,"2",strdup("dos"));

    char* value1 = (char*) hash_map_get(map,"1");
    char* value2 = (char*) hash_map_get(map,"2");
    char* value3 = (char*) hash_map_get(map,"3");

    CU_ASSERT_PTR_NULL(value3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(value2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(value1);

    CU_ASSERT_STRING_EQUAL("uno",value1);
    CU_ASSERT_STRING_EQUAL("dos",value2);

    free(value2);

    hash_map_put(map,"2",strdup("DOS"));
    value2 = hash_map_get(map,"2");

    CU_ASSERT_PTR_NOT_NULL_FATAL(value2);
    CU_ASSERT_STRING_EQUAL(value2,"DOS");

    hash_map_clean_and_destroy_elements(map,free);
    

}

static void test_hash_map_size(void){
    hash_map_put(map,"1",strdup("uno"));
    hash_map_put(map,"2",strdup("dos"));
    CU_ASSERT_EQUAL(hash_map_size(map),2);

    hash_map_clean_and_destroy_elements(map,free);

    CU_ASSERT_EQUAL(hash_map_size(map),0);
}

static void test_hash_map_remove(void){
    hash_map_put(map,"1",strdup("uno"));
    hash_map_put(map,"2",strdup("dos"));
    hash_map_put(map,"3",strdup("tres"));

    hash_map_remove_and_destroy_element(map,"3",free);

    char* value3 = (char*) hash_map_get(map,"3");
    CU_ASSERT_PTR_NULL(value3);

    char* value2 = (char*) hash_map_get(map,"2");
    CU_ASSERT_PTR_NOT_NULL_FATAL(value2);

    CU_ASSERT_STRING_EQUAL(value2,"dos");
    CU_ASSERT_EQUAL(hash_map_size(map),2);

    hash_map_clean_and_destroy_elements(map,free);
}

static void test_hash_map_resizing(void){
    char n[5];
    int* x;
    int len = MAP_INITIAL_CAPACITY + 5;
    for(int i =1; i<= len ;i++){
        sprintf(n,"%d",i);
        x = malloc(sizeof(int));
        *x = i;
        hash_map_put(map,n,x);
    }

    CU_ASSERT_EQUAL(hash_map_size(map),15);
    CU_ASSERT_TRUE(map->capacity > MAP_INITIAL_CAPACITY);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"15"),15);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"14"),14);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"13"),13);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"12"),12);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"11"),11);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"10"),10);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"9"),9);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"8"),8);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"7"),7);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"6"),6);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"5"),5);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"4"),4);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"3"),3);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"2"),2);
    CU_ASSERT_EQUAL_FATAL(*(int*)hash_map_get(map,"1"),1);
    hash_map_clean_and_destroy_elements(map,free);
}

static unsigned long my_hash(const char* s){
    return 33 * s[0];
}




static void test_map_collision(void){

    //testing collisions with a fake hash function

    t_hash_map* other = hash_map_create_with_hash_function(my_hash);

    hash_map_put(other,"Alberto",strdup("1"));
    hash_map_put(other,"Alvaro",strdup("2"));
    hash_map_put(other,"Alf",strdup("3"));
    hash_map_put(other,"Ava",strdup("4"));
    hash_map_put(other,"bb",strdup("4"));
    hash_map_put(other,"bc",strdup("4"));
    hash_map_put(other,"bd",strdup("4"));
    hash_map_put(other,"b3",strdup("4"));

   char* bd = hash_map_remove(other,"bd");

   CU_ASSERT_STRING_EQUAL_FATAL(bd,"4");

   free(bd);

    CU_ASSERT_STRING_EQUAL_FATAL((char*) hash_map_get(other,"Alvaro"),"2");
    CU_ASSERT_STRING_EQUAL_FATAL((char*) hash_map_get(other,"Alberto"),"1");
    CU_ASSERT_STRING_EQUAL_FATAL((char*) hash_map_get(other,"Alf"),"3");
    CU_ASSERT_STRING_EQUAL_FATAL((char*) hash_map_get(other,"Ava"),"4");

    hash_map_destroy_and_destroy_elements(other,free);
}

static int init_suite(void){
    map = hash_map_create();
    return 0;
}

static int clean_suite(void){
    hash_map_destroy_and_destroy_elements(map,free);
    return 0;
}

CU_pSuite get_hash_map_suite(void){
    CU_pSuite suite = CU_add_suite("Hash map suite", init_suite, clean_suite);
    CU_add_test(suite,"Hash map: test of put", test_hash_map_put);
    CU_add_test(suite,"Hash map: test of size", test_hash_map_size);
    CU_add_test(suite,"Hash map test of remove",test_hash_map_remove);
    CU_add_test(suite,"Hash map test of resizing",test_hash_map_resizing);
    CU_add_test(suite,"Hash map test of collisions",test_map_collision);
    return suite;
}

