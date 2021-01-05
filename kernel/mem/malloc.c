#include <malloc.h>
#include <phymem.h>
#include <debug.h>
#include <util.h>

_u8* __malloc_heap = (_u8*)PHY_KERNEL_HEAP;

// struct must be 8-byte aligned
typedef struct m_block
{
    linked_list_node_t elem;
    int size;
    bool is_free;
} __attribute__((aligned(8))) m_block_t;

linked_list_t blocks = {0};

void* allocate_new_block(size_t bytes)
{
    void* esp;

    __asm__(
        "mov %%esp, %0" : "=r" (esp)
    );

    // Don't corrupt stack
    kassert((void*)(__malloc_heap + bytes) < esp);

    // 8-byte alignment
    size_t offset = align_8(__malloc_heap);

    if (offset > 0)
    {
        __malloc_heap += (8 - offset);
    }

    void* old = __malloc_heap;

    // move down heap pointer
    __malloc_heap += bytes;

    return old;
}

// World's simplest malloc function. 100% leak. Enough to be unblocked for now...
void* kmalloc(size_t bytes)
{
    // Scan blocks for a free block
    linked_list_node_t* curr = blocks.head;
    m_block_t* block = NULL;

    while (curr != blocks.head->prev)
    {
        m_block_t* b = list_entry(curr, m_block_t, elem);

        if (b->is_free)
        {
            // Check neighbors if they are free and coalesce
            if (curr->prev != curr)
            {
                m_block_t* b_last = list_entry(curr->prev, m_block_t, elem);

                if (b_last->is_free)
                {
                    int new_size = sizeof(m_block_t) + b_last->size + b->size;
                    m_block_t* new_block = b_last;
                    new_block->size = new_size;
                    b = new_block;
                    linked_list_node_t* temp = curr->prev;
                    linked_list_remove(&blocks, curr);
                    curr = temp;
                }
            }

            if (curr->next != curr)
            {
                m_block_t* b_next = list_entry(curr->next, m_block_t, elem);

                if (b_next->is_free)
                {
                    int new_size = sizeof(m_block_t) + b_next->size + b->size;
                    b->size = new_size;
                    linked_list_remove(&blocks, curr->next);
                }
            }

            if (b->size >= bytes)
            {
                b->is_free = false;

                block = b;

                break;
            }
        }

        curr = curr->next;
    }

    if (block == NULL)
    {
        // No large enough free blocks found, create a new one
        void* data = allocate_new_block(sizeof(m_block_t) + bytes);

        block = (m_block_t*)data;

        block->size = bytes;
        block->is_free = false;
        linked_list_add_back(&blocks, &block->elem);
    }

    return ((_u8*)block)+sizeof(m_block_t);
}

void* kcalloc(size_t bytes)
{
    void* mem = kmalloc(bytes);
    memset(mem, 0, bytes);
}

void free(void* addr)
{
    if (addr == NULL)
    {
        return;
    }

    m_block_t* block = ((_u8*)addr)-sizeof(m_block_t);
    block->is_free = true;
}