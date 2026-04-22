#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "../src/server/client.h"

extern unsigned int client_count;

void setUp(void) {
    client_count = 0;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i] = NULL;
    }
}

void tearDown(void) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i] != NULL) {
            free(clients[i]);
            clients[i] = NULL;
        }
    }
    client_count = 0;
}

void test_enqueue_increments_count(void) {
    client_t *c = malloc(sizeof(client_t));
    strcpy(c->name, "Alice");
    enqueue(c);

    TEST_ASSERT_EQUAL_INT(1, client_count);
    TEST_ASSERT_EQUAL_PTR(c, clients[0]);
}

void test_dequeue_removes_client_from_array(void) {
    client_t *c = malloc(sizeof(client_t));
    strcpy(c->name, "Alice");
    enqueue(c);

    dequeue(c);

    TEST_ASSERT_EQUAL_INT(0, client_count);
}

void test_dequeue_clears_slot(void) {
    client_t *c = malloc(sizeof(client_t));
    strcpy(c->name, "Bob");
    enqueue(c);

    dequeue(c);

    TEST_ASSERT_NULL(clients[0]);
}

void test_dequeue_multiple_clients(void) {
    client_t *c1 = malloc(sizeof(client_t));
    client_t *c2 = malloc(sizeof(client_t));
    client_t *c3 = malloc(sizeof(client_t));
    strcpy(c1->name, "Alice");
    strcpy(c2->name, "Bob");
    strcpy(c3->name, "Charlie");

    enqueue(c1);
    enqueue(c2);
    enqueue(c3);

    TEST_ASSERT_EQUAL_INT(3, client_count);

    dequeue(c2);

    TEST_ASSERT_EQUAL_INT(2, client_count);
    TEST_ASSERT_EQUAL_PTR(c1, clients[0]);
    TEST_ASSERT_EQUAL_PTR(c3, clients[1]);
}

void test_dequeue_first_client(void) {
    client_t *c1 = malloc(sizeof(client_t));
    client_t *c2 = malloc(sizeof(client_t));
    strcpy(c1->name, "First");
    strcpy(c2->name, "Second");

    enqueue(c1);
    enqueue(c2);

    dequeue(c1);

    TEST_ASSERT_EQUAL_INT(1, client_count);
    TEST_ASSERT_EQUAL_PTR(c2, clients[0]);
}

void test_dequeue_last_client(void) {
    client_t *c1 = malloc(sizeof(client_t));
    client_t *c2 = malloc(sizeof(client_t));
    strcpy(c1->name, "First");
    strcpy(c2->name, "Second");

    enqueue(c1);
    enqueue(c2);

    dequeue(c2);

    TEST_ASSERT_EQUAL_INT(1, client_count);
    TEST_ASSERT_EQUAL_PTR(c1, clients[0]);
}