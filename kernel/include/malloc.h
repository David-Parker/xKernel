#pragma once

#include <stddef.h>
#include <util.h>
#include <ds.h>

// struct must be 8-byte aligned
typedef struct m_block
{
    linked_list_node_t elem;
    int size;
    bool is_free;
} __attribute__((aligned(8))) m_block_t;

#define MALLOC_BLOCK_SUB_MIN (sizeof(m_block_t) * 3)

static void* allocate_from(size_t bytes, _u8** start, _u8* end);

void* kmalloc_from(size_t bytes, _u8** start, _u8* end, linked_list_t* list);
void* kmalloc(size_t bytes);
void* kcalloc_from(size_t bytes, _u8** start, _u8* end, linked_list_t* list);
void* kcalloc(size_t bytes);
void free(void* addr);