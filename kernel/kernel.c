#include <intr.h>
#include <gdt.h>
#include <idt.h>
#include <console.h>
#include <keyboard.h>
#include <timer.h>
#include <tsc.h>
#include <msr.h>
#include <iolib.h>
#include <phymem.h>
#include <shell.h>
#include <util.h>

#ifdef UNIT_TEST
#include <test/unit_test.h>
#endif

// Kernel entry is jumped to from boot loader after enabling 32-bit prot execution. At this time virtual memory paging is not enabled.
void kmain() {
    stack_init();
    gdt_init();
    idt_init();
    intr_init();
    timer_init();
    keyboard_init();
    console_init();
    intr_enable();
   // get_tsc_freq();

    #ifdef UNIT_TEST
        test_driver();
    #endif
        shell_print_sig();
        kprintf("Welcome to xKernel! Type 'help' to get started.\n");
        //_u64 tsc_freq = get_tsc_freq();

        //kprintf("TSC Freq: %llu\n", tsc_freq);

        while (true)
        {
            shell_handle_user_input(&user_input);
            // if (timer_ticks % 18 == 0)
            // {
            //     kprintf("Ticks: %llu\n", timer_ticks);
            // }
            // curr = read_tsc();

            // if (curr - last >= tsc_freq / 16)
            // {
            //     //kprintf("Another second has passed...\n");
            //     last = curr;

            //     // if (dir == 0)
            //     //     console_scroll_up();
            //     // else
            //     //     console_scroll_down();
            // }
            // else if (curr - last_second >= tsc_freq)
            // {
            //     dir = !dir;
            //     last_second = curr;
            // }
        }

    // Don't let eip loose through memory...
    while(1);
    halt();
}