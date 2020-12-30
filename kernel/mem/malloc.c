#include <kernel/mem/malloc.h>
#include <kernel/mem/phymem.h>
#include <kernel/util/debug.h>
#include <kernel/util/util.h>

_u8* __malloc_heap = (_u8*)PHY_KERNEL_HEAP;

// World's simplest malloc function. 100% leak. Enough to be unblocked for now...
void* kmalloc(size_t bytes)
{
    void* esp;

    __asm__(
        "mov %%esp, %0" : "=r" (esp)
    );

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

void* kcalloc(size_t bytes)
{
    void* mem = kmalloc(bytes);
    memset(mem, 0, bytes);
}

void free(_u8* addr)
{
    kassert(addr != NULL);
}