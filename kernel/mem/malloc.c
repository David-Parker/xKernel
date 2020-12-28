#include <kernel/mem/malloc.h>
#include <kernel/mem/phymem.h>
#include <kernel/util/debug.h>

void* heap = (void*)PHY_KERNEL_HEAP_START;

// World's simplest malloc function. 100% leak. Enough to be unblocked for now...
void* kmalloc(size_t size)
{
    void* esp;

    __asm__(
        "mov %%esp, %0" : "=r" (esp)
    );

    kassert(heap-size > esp);

    void* old = heap;
    heap -= size;

    return old;
}

void* kcalloc(size_t size)
{
    void* mem = kmalloc(size);
    mem_set(mem, 0, size);
}