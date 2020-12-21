#pragma once

#include <kernel/util/stddef.h>

void memcopy(char *source, char *dest, int nbytes);
void halt();
void call_cpuid(
    unsigned int cpuid,
    unsigned int* eax,
    unsigned int* ecx,
    unsigned int* ebx,
    unsigned int* edx);