#pragma once

#include <kernel/util/stddef.h>
#include <kernel/util/util.h>
#include <kernel/cpu/intr.h>
#include <kernel/lib/iolib.h>

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