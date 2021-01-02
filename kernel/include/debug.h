#pragma once

#include <stddef.h>
#include <util.h>
#include <intr.h>
#include <iolib.h>

#define kassert(cond) \
do { \
    if (!(cond)) { \
        char buf[512]; \
        ksprintf("Assertion failed at %s %s:%d", buf, ARRSIZE(buf), __FILE__, __func__, __LINE__); \
        kpanic(buf);\
    } \
} \
while (0);

void kpanic(char* message);