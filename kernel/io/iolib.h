#pragma once

#include <stdarg.h>
#include <kernel/util/stddef.h>
#define PRINTF_BUFFER_MAX 4096

void reverse(char str[], int length);
char* safe_copy_str(char *destination, const char *source, char *end);
char* safe_copy_char(char* destination, const char c, char* end);
char* itoa(char *destination, char *end, int c, int base);
char* itoa_unsigned(char *destination, char *end, unsigned int c, int base);
char* ltoa(char* destination, char* end, long c, int base);
char* ltoa_unsigned(char* destination, char* end, unsigned long c, int base);
char* lltoa(char* destination, char* end, long long c, int base);
char* lltoa_unsigned(char* destination, char* end, unsigned long long c, int base);
void kprintf(const char *fmt, ...);
void vkprintf(const char *fmt, va_list args);