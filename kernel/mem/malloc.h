#pragma once

#include <kernel/util/stddef.h>
#include <kernel/util/util.h>

_u8* __malloc_heap;

void* kmalloc(size_t bytes);
void* kcalloc(size_t bytes);
void free(_u8* addr);