#pragma once

#include <kernel/util/stddef.h>

#define kassert(cond) \
do { \
    if (!(cond)) { \
        char buff[4096]; \
        ksprintf("Assertion failed at %s %s:%d\n", buff, 4096, __FILE__, __func__, __LINE__); \
        kpanic(buff);\
    } \
} \
while (0);

void kpanic(char* message);