#pragma once

#include <kernel/util/stddef.h>

void memcpy(char *source, char *dest, int nbytes);
void memset(_u8 *dest, _u8 val, _u32 len);
bool strcmp(char* lhs, char* rhs);
int strlen(char* str);
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