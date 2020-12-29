#include <kernel/lib/iolib.h>
#include <kernel/util/util.h>
#include <kernel/hw/console.h>
#include <kernel/hw/tsc.h>
#include <kernel/hw/msr.h>
#include <kernel/cpu/intr.h>
#include <kernel/cpu/gdt.h>
#include <kernel/cpu/idt.h>

#ifdef UNIT_TEST
#include <kernel/test/unit_test.h>
#endif

// Kernel entry is jumped to from boot loader after enabling 32-bit prot execution. At this time virtual memory paging is not enabled.
void main() {
    gdt_init();
    idt_init();
    console_init();

    #ifdef UNIT_TEST
        test_driver();
    #else
        kprintf("Hello %s, this is a test of printf! %d\n", "World", 42);
    #endif

        // Scroll the screen for a bit
        _u64 tsc_freq = get_tsc_freq();

        // kprintf("TSC Freq: %llu\n", tsc_freq);

        _u64 last = read_tsc();
        _u64 last_second = last;
        _u64 curr = 0;
        bool dir = 0;

        while (true)
        {
            curr = read_tsc();

            if (curr - last >= tsc_freq / 16)
            {
                //kprintf("Another second has passed...\n");
                last = curr;

                if (dir == 0)
                    console_scroll_down();
                else
                    console_scroll_up();
            }
            else if (curr - last_second >= tsc_freq)
            {
                dir = !dir;
                last_second = curr;
            }
        }

    // Don't let eip loose through memory...
    halt();
}