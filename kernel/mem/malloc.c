#include <kernel/mem/malloc.h>
#include <kernel/mem/phymem.h>
#include <kernel/util/debug.h>

_u8* __malloc_heap = (_u8*)PHY_KERNEL_HEAP_START;

// World's simplest malloc function. 100% leak. Enough to be unblocked for now...
void* kmalloc(size_t bytes)
{
    void* esp;

    __asm__(
        "mov %%esp, %0" : "=r" (esp)
    );

    kassert((void*)(__malloc_heap - bytes) > esp);

    // 8-byte alignment
    _u32 offset = (8 - ((_u32)__malloc_heap % 8));
    __malloc_heap -= offset;

    void* old = __malloc_heap;
    __malloc_heap -= bytes;

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