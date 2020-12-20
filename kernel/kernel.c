#include <kernel/io/iolib.h>

void main() {
    console_init();
    kprintf("Hello %s, this is a test of printf! %d\n", "World", 42);
    kprintf("New line\n");

    for (int i = 0; i < 100; ++i)
    {
        kprintf("%i ", i);
    }

    while(1);
}