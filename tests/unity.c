#include "unity.h"
#include <stdio.h>
#include <stdlib.h>

int unity_test_count = 0;
int unity_fail_count = 0;
static const char *current_file = NULL;

void unity_init(void) {
    unity_test_count = 0;
    unity_fail_count = 0;
    current_file = NULL;
}

void unity_finish(void) {
    printf("\n\n");
    if (unity_fail_count > 0) {
        printf("-----------------------\n");
        printf("Failures: %d\n", unity_fail_count);
        printf("Tests: %d\n", unity_test_count);
        exit(1);
    }
    printf("-----------------------\n");
    printf("All tests passed (%d)\n", unity_test_count);
}

void unity_begin(const char *pre) {
    unity_init();
    current_file = pre;
    printf("Running tests in %s...\n", pre);
}

void unity_end(const char *post) {
    (void)post;
    unity_finish();
}

void unity_failmessage(const char *msg, unsigned int line) {
    unity_fail_count++;
    printf("\nFAIL: %s (line %u)\n", msg, line);
}