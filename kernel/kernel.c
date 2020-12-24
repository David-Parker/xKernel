#include <kernel/lib/iolib.h>
#include <kernel/util/util.h>
#include <kernel/hw/console.h>
#include <kernel/hw/tsc.h>
#include <kernel/hw/msr.h>
#include <kernel/cpu/intr.h>
#include <kernel/cpu/gdt.h>
#include <kernel/cpu/idt.h>
#include <kernel/test/unit_test.h>

void div_by_zero(registers_t* regs)
{
    kprintf("Divide by zero!\n");
    regs->eip++;
}

// Kernel entry is jumped to from boot loader after enabling 32-bit prot execution. At this time virtual memory paging is not enabled.
void main() {
    gdt_init();
    idt_init();
    console_init();

    #ifdef UNIT_TEST
        test_init();

        int passed = 0;

        for (int i = 0; i < test_no; ++i)
        {
            if (unit_tests[i].func() == true)
            {
                passed++;
                console_set_colors(VGA_COLOR_BLACK, VGA_COLOR_GREEN);
                kprintf("PASSED ");
                console_set_colors(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
                kprintf("%s()\n", unit_tests[i].str);
            }
            else
            {
                console_set_colors(VGA_COLOR_BLACK, VGA_COLOR_RED);
                kprintf("FAILED ");
                console_set_colors(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
                kprintf("%s()\n", unit_tests[i].str);
            }
        }

        kprintf("%d out of %d tests passed.\n", passed, test_no);

        halt();
    #else
        intr_register_handler(0, div_by_zero);

        // Divide by zero, this should call the DIV_ZERO IRQ (0) - if no handler is set, CPU will get stuck in a reboot loop
        int x = 2 / 0;
        
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
    #endif
}