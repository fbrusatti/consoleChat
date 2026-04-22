#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern int unity_test_count;
extern int unity_fail_count;

void unity_init(void);
void unity_finish(void);
void unity_begin(const char *pre);
void unity_end(const char *post);
void unity_failmessage(const char *msg, unsigned int line);
void unity_floatize(int is_float);

#define UNITY_BEGIN() unity_begin(__FILE__)
#define UNITY_END() unity_end(__FILE__)

#define RUN_TEST(func, line) do { \
    unity_test_count++; \
    setUp(); \
    func(); \
    tearDown(); \
    printf("."); \
    fflush(stdout); \
} while(0)

#define TEST_ASSERT_EQUAL_INT(expected, actual) do { \
    int exp_ = (expected); \
    int act_ = (actual); \
    if (exp_ != act_) { \
        unity_failmessage("Expected " #expected " but got " #actual, __LINE__); \
    } \
} while(0)

#define TEST_ASSERT_EQUAL_PTR(expected, actual) do { \
    void *exp_ = (expected); \
    void *act_ = (actual); \
    if (exp_ != act_) { \
        unity_failmessage("Expected pointer " #expected " but got " #actual, __LINE__); \
    } \
} while(0)

#define TEST_ASSERT_NULL(ptr) do { \
    if ((ptr) != NULL) { \
        unity_failmessage("Expected NULL but got " #ptr, __LINE__); \
    } \
} while(0)

#define TEST_ASSERT_TRUE(expr) do { \
    if (!(expr)) { \
        unity_failmessage("Expected " #expr " to be true", __LINE__); \
    } \
} while(0)

#define TEST_ASSERT_FALSE(expr) do { \
    if (expr) { \
        unity_failmessage("Expected " #expr " to be false", __LINE__); \
    } \
} while(0)

#endif