#include <kernel/util/util.h>

// TODO: more efficient to implement with SIMD
void memcopy(char *source, char *dest, int nbytes)
{
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void halt()
{
    __asm__("hlt");
}

void call_cpuid(
    unsigned int cpuid,
    unsigned int* eax,
    unsigned int* ecx,
    unsigned int* ebx,
    unsigned int* edx)
{
    __asm__(
        "cpuid"
        : "=a" (*eax),
        "=b" (*ebx),
        "=c" (*ecx),
        "=d" (*edx)
        : "0" (cpuid));
}