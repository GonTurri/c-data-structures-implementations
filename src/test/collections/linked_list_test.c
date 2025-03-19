#include "linked_list_test.h"

static t_linked_list *list;

static bool compare_ints(void *a, void *b)
{
    return (*(int *)a < *(int *)b);
}

static void add_random_elements(t_linked_list *list)
{
    int *a = malloc(sizeof(int));
    *a = 5;

    int *b = malloc(sizeof(int));
    *b = 3;

    int *c = malloc(sizeof(int));
    *c = 1;

    linked_list_add(list, a);
    linked_list_add(list, b);
    linked_list_add(list, c);
}

static void remove_random_elements(t_linked_list *list)
{
    linked_list_clean_and_destroy_elements(list, free);
}

// should add the elements 5,3,1 to the list

static void test_linked_list_add(void)
{
    int *buf;

    add_random_elements(list);

    CU_ASSERT(linked_list_get(list, 0, (void **)&buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf, 5);
    CU_ASSERT(linked_list_get(list, 1, (void **)&buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf, 3);
    CU_ASSERT(linked_list_get(list, 2, (void **)&buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf, 1);
    CU_ASSERT_EQUAL(linked_list_size(list), 3);
    remove_random_elements(list);
}

static void test_linked_list_add_first(void)
{

    int *buf;

    int *x = malloc(sizeof(int));
    *x = 10;

    add_random_elements(list);

    linked_list_add_first(list, x);

    linked_list_get(list, 0, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 10);

    remove_random_elements(list);
}

static void test_linked_list_add_to_index(void)
{
    add_random_elements(list);

    int *x = malloc(sizeof(int));
    *x = 10;

    linked_list_add_to_index(list, 1, x);

    CU_ASSERT_EQUAL(linked_list_size(list), 4);

    int *buf;
    linked_list_get(list, 1, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 10);

    int *y = malloc(sizeof(int));
    *y = 8;

    linked_list_add_to_index(list, 0, y);
    CU_ASSERT_EQUAL(*(int *)list->head->data, 8);

    int *z = malloc(sizeof(int));
    *z = 20;

    linked_list_add_to_index(list, 4, z);
    CU_ASSERT_EQUAL(*(int *)list->tail->data, 20);

    remove_random_elements(list);
}

static void test_linked_list_add_all(void)
{
    add_random_elements(list);
    t_linked_list *target = linked_list_create();
    int *buf;

    linked_list_add_all(list, target);
    CU_ASSERT_EQUAL(linked_list_size(target), 3);
    linked_list_get(target, 0, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 5);
    linked_list_get(target, 1, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 3);
    linked_list_get(target, 2, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 1);
    remove_random_elements(list);
    linked_list_destroy(target);
}

static bool find_condition_success(void *elem)
{
    return *((int *)elem) < 2;
}

static bool find_condition_failure(void *elem)
{
    return *((int *)elem) < 0;
}

static void test_linked_list_find(void)
{
    int *buf;
    add_random_elements(list);
    t_list_error err = linked_list_find(list, find_condition_success, (void **)&buf);
    CU_ASSERT_EQUAL(err, LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf, 1);
    remove_random_elements(list);
}

static void test_linked_list_find_not_found(void)
{
    int *buf;
    add_random_elements(list);
    t_list_error err = linked_list_find(list, find_condition_failure, (void **)&buf);
    CU_ASSERT_EQUAL(err, LIST_NOT_FOUND);
    remove_random_elements(list);
}

// should return LIST_INDEX_OUT_OF_BOUNDS when trying to get and out of bounds position

static void test_linked_list_get_errors(void)
{
    int *buf;

    add_random_elements(list);

    t_list_error err = linked_list_get(list, -1, (void **)&buf);
    CU_ASSERT_EQUAL(err, LIST_INDEX_OUT_OF_BOUNDS);
    err = linked_list_get(list, 3, (void **)&buf);
    CU_ASSERT_EQUAL(err, LIST_INDEX_OUT_OF_BOUNDS);

    remove_random_elements(list);
}

static void test_linked_list_remove_by_index(void)
{
    int *buf;
    add_random_elements(list);

    linked_list_remove(list, 1, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 3);
    CU_ASSERT_EQUAL(linked_list_size(list), 2);
    linked_list_remove(list, 0, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 5);
    CU_ASSERT_EQUAL(linked_list_size(list), 1);
    linked_list_remove(list, 0, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 1);
    CU_ASSERT(linked_list_is_empty(list));
}

static void test_linked_list_clean(void)
{
    int x = 5, y = 10;
    linked_list_add(list, &x);
    linked_list_add(list, &y);

    linked_list_clean(list);

    CU_ASSERT(linked_list_is_empty(list));
    CU_ASSERT_EQUAL(x, 5);
    CU_ASSERT_EQUAL(y, 10);
}

static void plus_one(void *elem)
{
    int *num = (int *)elem;
    (*num)++;
}

static void test_linked_list_foreach(void)
{
    add_random_elements(list);
    linked_list_foreach(list, plus_one);

    int *buf;

    CU_ASSERT(linked_list_get(list, 0, (void **)&buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf, 6);
    CU_ASSERT(linked_list_get(list, 1, (void **)&buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf, 4);
    CU_ASSERT(linked_list_get(list, 2, (void **)&buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf, 2);

    remove_random_elements(list);
}

static void test_linked_list_set(void)
{
    int *buf;
    int *x = malloc(sizeof(int));
    *x = 10;
    add_random_elements(list);
    linked_list_set(list, 0, x, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 5);
    CU_ASSERT(linked_list_get(list, 0, (void **)&buf) == LIST_SUCCESS);
    CU_ASSERT_EQUAL(*buf, 10);

    remove_random_elements(list);
}

static void test_linked_list_replace_by_condition(void)
{
    int *buf;
    int *x = malloc(sizeof(int));
    *x = 10;
    add_random_elements(list);
    linked_list_replace_by_condition(list, find_condition_success, x, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 1);
    CU_ASSERT(linked_list_find(list, find_condition_success, (void **)&buf) == LIST_NOT_FOUND);
    remove_random_elements(list);
}

static void test_linked_list_any_satisfy(void)
{
    add_random_elements(list);

    CU_ASSERT(linked_list_any_satisfy(list, find_condition_success));
    CU_ASSERT_FALSE(linked_list_any_satisfy(list, find_condition_failure));
    remove_random_elements(list);
}

static bool should_all_satisfy_condition(void *elem)
{
    return *((int *)elem) < 6;
}

static bool should_not_all_satisfy_condition(void *elem)
{
    return *((int *)elem) < 5;
}

static void test_linked_list_all_satisfy(void)
{
    add_random_elements(list);
    CU_ASSERT(linked_list_all_satisfy(list, should_all_satisfy_condition));
    CU_ASSERT_FALSE(linked_list_all_satisfy(list, should_not_all_satisfy_condition));
    remove_random_elements(list);
}

static void test_linked_list_count(void)
{
    add_random_elements(list);
    CU_ASSERT_EQUAL(linked_list_count(list, should_not_all_satisfy_condition), 2);
    remove_random_elements(list);
}

static void test_linked_list_duplicate(void)
{
    add_random_elements(list);
    t_linked_list *dup = linked_list_duplicate(list);

    int *buf;
    CU_ASSERT_EQUAL(linked_list_size(dup), 3);
    linked_list_get(dup, 0, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 5);
    linked_list_get(dup, 1, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 3);
    linked_list_get(dup, 2, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 1);
    remove_random_elements(list);

    linked_list_destroy(dup);
    remove_random_elements(list);
}

static void test_linked_list_filter(void)
{
    add_random_elements(list);
    t_linked_list *filter = linked_list_filter(list, should_not_all_satisfy_condition);
    CU_ASSERT_EQUAL(linked_list_size(filter), 2);
    int *buf;
    linked_list_get(filter, 1, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 1)
    linked_list_clean(list);
    remove_random_elements(list);
}

static void *new_plus_one(void *elem)
{
    int *num = (int *)elem;
    int *new = malloc(sizeof(int));
    *new = *num + 1;
    return new;
}

static void test_linked_list_map(void)
{
    add_random_elements(list);
    t_linked_list *mapped = linked_list_map(list, new_plus_one);
    int *buf;
    linked_list_get(mapped, 0, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 6)
    linked_list_get(mapped, 1, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 4)
    linked_list_get(mapped, 2, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 2)
    remove_random_elements(list);
}

static void test_linked_list_sort(void)
{
    add_random_elements(list);
    linked_list_sort(list, compare_ints);
    int *buf;
    linked_list_get(list, 0, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 1)
    linked_list_get(list, 1, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 3)
    linked_list_get(list, 2, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 5)
    remove_random_elements(list);
}

static void test_linked_list_sorted(void)
{
    add_random_elements(list);
    t_linked_list *sorted = linked_list_sorted(list, compare_ints);
    int *buf;
    linked_list_get(sorted, 0, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 1)
    linked_list_get(sorted, 1, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 3)
    linked_list_get(sorted, 2, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 5)

    linked_list_get(list, 0, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 5)
    linked_list_destroy(sorted);
    remove_random_elements(list);
}

static void test_linked_list_add_sorted(void)
{

    int *buf;

    int *x = malloc(sizeof(int));
    *x = 4;

    add_random_elements(list);

    linked_list_sort(list, compare_ints);

    int i = linked_list_add_sorted(list, x, compare_ints);

    linked_list_get(list, 2, (void **)&buf);
    CU_ASSERT_EQUAL(*buf, 4);
    CU_ASSERT_EQUAL(i, 2);

    int *y = malloc(sizeof(int));
    *y = 0;
    linked_list_add_sorted(list, y, compare_ints);
    CU_ASSERT_EQUAL(*(int *)list->head->data, 0);

    int *z = malloc(sizeof(int));
    *z = 20;
    linked_list_add_sorted(list, z, compare_ints);
    CU_ASSERT_EQUAL(*(int *)list->tail->data, 20);

    remove_random_elements(list);
}

static void test_linked_list_slice(void)
{
    add_random_elements(list);
    add_random_elements(list);

    t_linked_list *sliced = linked_list_slice(list, 2, 2);
    int *buf, *buf2;
    linked_list_get(sliced, 0, (void **)&buf);
    linked_list_get(sliced, 1, (void **)&buf2);
    CU_ASSERT_EQUAL(linked_list_size(sliced), 2);
    CU_ASSERT_EQUAL(*buf, 1);
    CU_ASSERT_EQUAL(*buf2, 5);
    linked_list_destroy(sliced);
    remove_random_elements(list);
}

static void test_linked_list_slice_and_remove(void)
{
    add_random_elements(list);
    add_random_elements(list);

    t_linked_list *sliced = linked_list_slice_and_remove(list, 2, 2);
    int *buf, *buf2, *buf3, *buf4;
    linked_list_get(sliced, 0, (void **)&buf);
    linked_list_get(sliced, 1, (void **)&buf2);
    linked_list_get(list, 2, (void **)&buf3);
    linked_list_get(list, 3, (void **)&buf4);

    CU_ASSERT_EQUAL(linked_list_size(sliced), 2);
    CU_ASSERT_EQUAL(linked_list_size(list), 4);
    CU_ASSERT_EQUAL(*buf, 1);
    CU_ASSERT_EQUAL(*buf2, 5);
    CU_ASSERT_EQUAL(*buf3, 3);
    CU_ASSERT_EQUAL(*buf4, 1);
    linked_list_destroy(sliced);
    remove_random_elements(list);
}

static int init_linked_list(void)
{
    list = linked_list_create();
    return 0;
}

static int destroy_linked_list(void)
{
    linked_list_destroy(list);
    return 0;
}

CU_pSuite get_linked_list_suite(void)
{
    CU_pSuite suite = CU_add_suite("Linked list suite", init_linked_list, destroy_linked_list);
    CU_add_test(suite, "test of linked_list_add()", test_linked_list_add);
    CU_add_test(suite, "test of linked_list_add_first()", test_linked_list_add_first);
    CU_add_test(suite, "test of linked_list_add_all()", test_linked_list_add_all);
    CU_add_test(suite, "test of linked_list_add_to_index()", test_linked_list_add_to_index);
    CU_add_test(suite, "test of linked_list_get() when index out of bounds", test_linked_list_get_errors);
    CU_add_test(suite, "test of linked_list_set()", test_linked_list_set);
    CU_add_test(suite, "test of linked_list_replace_by_condition()", test_linked_list_replace_by_condition);
    CU_add_test(suite, "test of linked_list_find() when found", test_linked_list_find);
    CU_add_test(suite, "test of linked_list_find() when not found", test_linked_list_find_not_found);
    CU_add_test(suite, "test of linked_list_remove()", test_linked_list_remove_by_index);
    CU_add_test(suite, "test of linked_list_clean()", test_linked_list_clean);
    CU_add_test(suite, "test of linked_list_foreach()", test_linked_list_foreach);
    CU_add_test(suite, "test of linked_list_any_satisfy()", test_linked_list_any_satisfy);
    CU_add_test(suite, "test of linked_list_all_satisfy()", test_linked_list_all_satisfy);
    CU_add_test(suite, "test of linked_list_count()", test_linked_list_count);
    CU_add_test(suite, "test of linked_list_duplicate()", test_linked_list_duplicate);
    CU_add_test(suite, "test of linked_list_filter()", test_linked_list_filter);
    CU_add_test(suite, "test of linked_list_map()", test_linked_list_map);
    CU_add_test(suite, "test of linked_list_sort()", test_linked_list_sort);
    CU_add_test(suite, "test of linked_list_sorted()", test_linked_list_sorted);
    CU_add_test(suite, "test of linked_list_add_sorted()", test_linked_list_add_sorted);
    CU_add_test(suite, "test of linked_list_slice()", test_linked_list_slice);
    CU_add_test(suite, "test of linked_list_slice_and_remove()", test_linked_list_slice_and_remove);
    return suite;
}
