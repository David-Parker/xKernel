#include <kernel/lib/iolib.h>
#include <kernel/util/util.h>
#include <kernel/hw/console.h>
#include <kernel/hw/tsc.h>
#include <kernel/hw/msr.h>
#include <kernel/cpu/gdt.h>
#include <kernel/cpu/idt.h>

// Kernel entry is jumped to from boot loader after enabling 32-bit prot execution. At this time virtual memory paging is not enabled.
void main() {
    gdt_init();
    idt_init();
    console_init();

    // Divide by zero, this should call the DIV_ZERO IRQ (0) - if no handler is set, CPU will get stuck in a reboot loop
    int x = 2 / 0;
   // __asm__ ("int $0x00");
    
    kprintf("Hello %s, this is a test of printf! %d\n", "World", 42);

    _u64 tsc_freq = get_tsc_freq();

    kprintf("TSC Freq: %llu\n", tsc_freq);

    _u64 last = read_tsc();
    _u64 curr = 0;

    while (true)
    {
        curr = read_tsc();

        if (curr - last >= tsc_freq)
        {
            kprintf("Another second has passed...\n");
            last = curr;
        }
    }

    // Don't let eip loose through memory...
    halt();
}