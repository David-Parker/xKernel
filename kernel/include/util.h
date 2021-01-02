#pragma once

#include <stddef.h>

#define align_4(addr) ((size_t)addr & 0x3)
#define align_8(addr) ((size_t)addr & 0x7)

void * memcpy (void * destination, const void * source, size_t num );
void * memset (void * ptr, int value, size_t num );
void halt();
int min(int lhs, int rhs);
int max(int lhs, int rhs);
int modulo(int a, int b);

/*
    void * memset ( void * ptr, int value, size_t num );
    void * memcpy ( void * destination, const void * source, size_t num );
    void * memmove ( void * destination, const void * source, size_t num );
    int memcmp ( const void * ptr1, const void * ptr2, size_t num );
*/