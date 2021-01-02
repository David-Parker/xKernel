#pragma once

#include <stddef.h>
#include <util.h>

void* kmalloc(size_t bytes);
void* kcalloc(size_t bytes);
void free(void* addr);