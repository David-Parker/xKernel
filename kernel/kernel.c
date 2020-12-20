#include <kernel/io/iolib.h>

void stop()
{
    __asm__("hlt");
}

void main() {
    console_init();
    kprintf("Hello %s, this is a test of printf! %d\n", "World", 42);
    kprintf("New line\n");

    // Don't let eip loose through memory...
    stop();
}