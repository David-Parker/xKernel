#include <kernel/io/iolib.h>

void main() {
    kprintf("Hello %s, this is a test of printf! %d", "World", 42);

    while(1);
}