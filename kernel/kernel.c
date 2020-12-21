#include <kernel/io/iolib.h>
#include <kernel/util/util.h>
#include <kernel/hw/console.h>
#include <kernel/hw/tsc.h>
#include <kernel/hw/msr.h>

// Kernel entry is jumped to from boot loader after enabling 32-bit prot execution. At this time virtual memory paging is not enabled.
void main() {
    console_init();
    kprintf("Hello %s, this is a test of printf! %d\n", "World", 42);
    kprintf("New line\n");

    msr_supported();

    //kprintf("MSRs: %d\n", msr_supported());

    _u64 tsc_freq = get_tsc_freq();

    //kprintf("TSC Freq: %llu", tsc_freq);

    // _u64 curr = read_tsc();
    // _u64 cycles_per_second_guess = 3490000000ull;
    // _u64 ten_seconds = 10 * cycles_per_second_guess;

    // kprintf("Start\n");
    // while ((read_tsc() - curr) < ten_seconds)
    // {

    // }

    // kprintf("Done\n");

    // if (rdtscp_supported())
    // {
    //     kprintf("TSCP: %llu", read_tscp());
    // }
    // else
    // {
    //     kprintf("TSC: %llu\n", read_tsc());
    //     kprintf("TSC: %llu\n", read_tsc());
    //     kprintf("TSC: %llu\n", read_tsc());
    // }

    // Don't let eip loose through memory...
    halt();
}