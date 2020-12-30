#pragma once

#include <kernel/util/stddef.h>

typedef bool (*test_func)(void);

typedef struct
{
    test_func func;
    const char* str;
} unit_test_func;

int test_no;
unit_test_func unit_tests[256];

#define TEST_FUNC(name) \
do { \
    unit_tests[test_no].func = name; \
    unit_tests[test_no].str = #name; \
    test_no++; \
} \
while(0);

#define tassert(cond) \
do { \
    if (!(cond)) { \
        kprintf("Test assert failed: %s %s:%d\n", __FILE__, __func__, __LINE__); \
        return false; \
    } \
} \
while (0);

void test_init();
void test_driver();