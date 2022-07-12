#include <intr.h>
#include <gdt.h>
#include <idt.h>
#include <console.h>
#include <keyboard.h>
#include <time.h>
#include <timer.h>
#include <tsc.h>
#include <msr.h>
#include <iolib.h>
#include <phymem.h>
#include <shell.h>
#include <util.h>
#include <tty.h>

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
    tty_init(&tty_default, 1024, VGA_MAX_ROWS - 1, VGA_MAX_COLS);
    console_init(&tty_default);
    intr_enable();
    tsc_init();

    while (true)
    {
        //kprintf("The time is %llu\n", ktime_get_ns());
        shell_handle_user_input(&user_input);
        halt();
    }
}