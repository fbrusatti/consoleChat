#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"

void setUp(void);
void tearDown(void);

void test_enqueue_increments_count(void);
void test_dequeue_removes_client_from_array(void);
void test_dequeue_clears_slot(void);
void test_dequeue_multiple_clients(void);
void test_dequeue_first_client(void);
void test_dequeue_last_client(void);

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_enqueue_increments_count, __LINE__);
    RUN_TEST(test_dequeue_removes_client_from_array, __LINE__);
    RUN_TEST(test_dequeue_clears_slot, __LINE__);
    RUN_TEST(test_dequeue_multiple_clients, __LINE__);
    RUN_TEST(test_dequeue_first_client, __LINE__);
    RUN_TEST(test_dequeue_last_client, __LINE__);
    UNITY_END();
    return 0;
}