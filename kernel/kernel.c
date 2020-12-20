#include <kernel/io/iolib.h>
#include <kernel/util/util.h>

// Kernel entry is jumped to from boot loader after enabling 32-bit prot execution. At this time virtual memory paging is not enabled.
void main() {
    console_init();
    kprintf("Hello %s, this is a test of printf! %d\n", "World", 42);
    kprintf("New line\n");

    // Don't let eip loose through memory...
    halt();
}