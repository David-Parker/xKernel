#include <kernel/hw/tsc.h>
#include <kernel/hw/time.h>
#include <kernel/io/iolib.h>
#include <kernel/util/util.h>

// All about TSC: https://unix4lyfe.org/benchmarking/
_u64 read_tscp()
{
    unsigned lo, hi;
    __asm__("rdtscp"
        : "=a" (lo), "=d" (hi)
        :
        : ); // ignore ecx value and clobber
    return ((_u64)lo) | (((_u64)hi) << 32);
}

_u64 read_tsc()
{
    unsigned lo, hi;
    __asm__("rdtsc"
        : "=a" (lo), "=d" (hi)
        :
        : ); // ignore ecx value and clobber
    return ((_u64)lo) | (((_u64)hi) << 32);
}

int rdtscp_supported() {
    unsigned a, b, c, d;
    call_cpuid(0x80000001, &a, &b, &c, &d);

    if (d & (1 << 27))
    {
        // RDTSCP is supported.
        return 1;
    }
    else
    {
        // RDTSCP is not supported.
        return 0;
    }
}

_u64 get_tsc_freq()
{
    _u64 cyclesPerSec = 0;
    _u64 gap = 0;
    _u32 index;
    _u64 time;
    _u64 time1;
    _u64 time2;
    _u64 tsc0;
    _u64 tsc1;
    _u64 tsc2;

    for (index = 0; index < 3; index += 1)
    {
        tsc0 = read_tsc();
        time = ktime_get_ns();

        do {
            time1 = ktime_get_ns();
        } while (time1 == time);

        tsc1 = read_tsc();

        do {
            time2 = ktime_get_ns();
        } while (time2 - time1 < 1 * NANOS_PER_SEC);

        tsc2 = read_tsc();

        if ((gap == 0) ||
            ((tsc2 > tsc1) && (tsc2 - tsc0 < gap))) {

            gap = tsc2 - tsc0;
            cyclesPerSec = (tsc2 - tsc1) * (_u64)(NANOS_PER_SEC) / (time2 - time1);

            // round to nearest mhz
            cyclesPerSec += 1000000 / 2;
            cyclesPerSec -= (cyclesPerSec % 1000000);
        }
    }

    return cyclesPerSec;
}