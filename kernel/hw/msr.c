#include <kernel/hw/msr.h>
#include <kernel/hw/cpuid.h>

bool msr_supported() {
    _u32 a = 0;
    _u32 b = 0;
    _u32 c = 0;
    _u32 d = 0;

    call_cpuid(0x01, 0, &a, &b, &c, &d);

    if (d & 0x20)
    {
        // MSRs are supported.
        return true;
    }
    else
    {
        // MSRs are not supported.
        return false;
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

    return (((_u64)edx) << 32 | ((_u64)eax & 0xFFFFFFFF));
}