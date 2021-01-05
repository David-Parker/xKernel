#pragma once

#include <stddef.h>
#include <util.h>
#include <ds.h>

#define MALLOC_BLOCK_SUB_MIN 64 

void* kmalloc(size_t bytes);
void* kcalloc(size_t bytes);
void free(void* addr);