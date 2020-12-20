#include <kernel/util/iolib.h>

void main() {
    kprintf("Hello %s, this is a test of printf! %d", "World", 42);
    // kprintf("Sup\n");

    while(1);
}