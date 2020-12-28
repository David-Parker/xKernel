#pragma once

#include <kernel/util/stddef.h>
#include <kernel/util/util.h>

void* heap;

void* kmalloc(size_t size);
void* kcalloc(size_t size);