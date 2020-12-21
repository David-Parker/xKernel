#pragma once

#include <kernel/util/stddef.h>

void memcopy(char *source, char *dest, int nbytes);
void halt();
void call_cpuid(
    _u32 cpuid,
    _u32* eax,
    _u32* ecx,
    _u32* ebx,
    _u32* edx);