#include <malloc.h>
#include <phymem.h>
#include <debug.h>
#include <util.h>

_u8* __program_break = (_u8*)PHY_KERNEL_HEAP;

linked_list_t blocks = {0};

void* allocate_from(size_t bytes, _u8** start, _u8* end)
{
    _u8* temp = *start;

    // 8-byte alignment
    size_t offset = align_8(temp);

    if (offset > 0)
    {
        temp += (8 - offset);
    }

    void* old = temp;

    // move down heap pointer
    temp += bytes;

    if (temp > end)
    {
        // Error
        return NULL;
    }
    else
    {
        *start = temp;
        return old;
    }
}

void* kmalloc_from(size_t bytes, _u8** start, _u8* end, linked_list_t* list)
{
    // Scan blocks for a free block
    linked_list_node_t* curr = list->head;
    m_block_t* block = NULL;

    size_t offset = align_8(bytes);

    if (offset > 0)
    {
        bytes += (8 - offset);
    }

    do
    {
        if (list->count == 0)
        {
            break;
        }

        m_block_t* b = list_entry(curr, m_block_t, elem);

        if (b->is_free)
        {
            // Check neighbors if they are free and coalesce
            while (curr->next != curr)
            {
                m_block_t* b_next = list_entry(curr->next, m_block_t, elem);

                if (b_next->is_free)
                {
                    int new_size = sizeof(m_block_t) + b_next->size + b->size;
                    b->size = new_size;
                    linked_list_remove(list, curr->next);
                }
                else
                {
                    break;
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
    while (curr != list->head->prev);

    if (block == NULL)
    {
        // No large enough free blocks found, create a new one
        void* data = allocate_from(sizeof(m_block_t) + bytes, start, end);

        // No room to allocate, error
        if (data == NULL)
        {
            return NULL;
        }

        block = (m_block_t*)data;

        block->size = bytes;
        block->is_free = false;
        linked_list_add_back(list, &block->elem);
    }
    else
    {
        // Check if we should subdivide
        if (block->size > MALLOC_BLOCK_SUB_MIN + bytes)
        {
            kassert(list->count > 0);

            int old_size = block->size;
            block->size = bytes;

            _u8* pNew_block = (_u8*)block;
            pNew_block += sizeof(m_block_t);
            pNew_block += bytes;

            // Ensure 8-byte alignment
            kassert((size_t)pNew_block % 8 == 0);

            m_block_t* new_block = (m_block_t*)pNew_block;
            new_block->is_free = true;
            new_block->size = old_size - bytes - sizeof(m_block_t);

            linked_list_add_after(list, &block->elem, &new_block->elem);
        }
    }

    return ((_u8*)block)+sizeof(m_block_t);
}

void* kmalloc(size_t bytes)
{
    return kmalloc_from(bytes, &__program_break, (_u8*)PHY_KERNEL_STACK, &blocks);
}

void* kcalloc_from(size_t bytes, _u8** start, _u8* end, linked_list_t* list)
{
    void* mem = kmalloc_from(bytes, start, end, list);
    memset(mem, 0, bytes);
}

void* kcalloc(size_t bytes)
{
    return kcalloc_from(bytes, &__program_break, (_u8*)PHY_KERNEL_STACK, &blocks);
}

void free(void* addr)
{
    if (addr == NULL)
    {
        return;
    }

    _u8* pAddr = (_u8*)addr;
    pAddr -= sizeof(m_block_t);

    m_block_t* block = (m_block_t*)pAddr;
    block->is_free = true;
}