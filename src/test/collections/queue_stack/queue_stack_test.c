#include "queue_stack_test.h"

static bool is_palindrome(char *word)
{
    t_stack *stack = stack_create();
    t_queue *queue = queue_create();
    char *a, *b;
    int len = strlen(word);
    if (!word || 0 == len)
        return false;
    for (int i = 0; i < len; i++)
    {
        stack_push(stack, &word[i]);
        queue_push(queue, &word[i]);
    }

    while (!stack_is_empty(stack) && !queue_is_empty(queue))
    {
        stack_pop(stack, (void **)&a);
        queue_pop(queue, (void **)&b);
        if (*a != *b)
        {
            stack_destroy(stack);
            queue_destroy(queue);
            return false;
        }
    }

    stack_destroy_and_destroy_elements(stack, free);
    queue_destroy_and_destroy_elements(queue, free);
    return true;
}

static void test_stack_and_queue_palindrome(void)
{
    CU_ASSERT(is_palindrome("neuquen"));
    CU_ASSERT_FALSE(is_palindrome("jorge"));
}

CU_pSuite get_queue_stack_suite(void)
{
    CU_pSuite suite = CU_add_suite("queue and stack suite", NULL, NULL);
    CU_ADD_TEST(suite, test_stack_and_queue_palindrome);
    return suite;
}
