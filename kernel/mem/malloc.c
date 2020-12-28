#include <kernel/mem/malloc.h>
#include <kernel/mem/phymem.h>
#include <kernel/util/debug.h>

_u8* heap = (_u8*)PHY_KERNEL_HEAP_START;

// World's simplest malloc function. 100% leak. Enough to be unblocked for now...
void* kmalloc(size_t bytes)
{
    void* esp;

    __asm__(
        "mov %%esp, %0" : "=r" (esp)
    );

    kassert((void*)(heap - bytes) > esp);

    void* old = heap;
    heap -= bytes;

    return old;
}

void* kcalloc(size_t bytes)
{
    void* mem = kmalloc(bytes);
    mem_set(mem, 0, bytes);
}

void free(_u8* addr)
{
    kassert(addr != NULL);
}