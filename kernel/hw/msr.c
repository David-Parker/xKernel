#include <kernel/hw/msr.h>
#include <kernel/io/iolib.h>

int msr_supported() {
    unsigned a, b, c, d;
    call_cpuid(0x00, &a, &b, &c, &d);

    kprintf("CPUID 0: %u %u %u %u\n", a, b, c, d);

    call_cpuid(0x15, &a, &b, &c, &d);

    kprintf("CPUID 0: %u %u %u %u\n", a, b, c, d);

    if (d & (1 << 6))
    {
        // MSRs are supported.
        return 1;
    }
    else
    {
        // MSRs are not supported.
        return 0;
    }
}

_u64 read_msr(_u32 msr)
{
    _u32 edx, eax;

    __asm__(
        "rdmsr"
        : 
            "=a" (eax),
            "=d" (edx)
        : 
            "c" (msr)
    );

    return ((_u64)eax) | (((_u64)edx) << 32);
}