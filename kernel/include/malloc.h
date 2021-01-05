#pragma once

#include <stddef.h>
#include <util.h>
#include <ds.h>

void* kmalloc(size_t bytes);
void* kcalloc(size_t bytes);
void free(void* addr);