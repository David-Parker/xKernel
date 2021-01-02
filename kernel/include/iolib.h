#pragma once

#include <stdarg.h>
#include <stddef.h>
#define PRINTF_BUFFER_MAX 4096
#define isdigit(c) ((unsigned) ((c) - '0') < 10U)

void reverse(char str[], int length);
char* safe_copy_str(char *destination, const char *source, char *end);
char* safe_copy_char(char* destination, const char c, char* end);
char* itoa(char *destination, char *end, int c, int base);
char* itoa_unsigned(char *destination, char *end, unsigned int c, int base);
char* ltoa(char* destination, char* end, long c, int base);
char* ltoa_unsigned(char* destination, char* end, unsigned long c, int base);
char* lltoa(char* destination, char* end, long long c, int base);
char* lltoa_unsigned(char* destination, char* end, unsigned long long c, int base);
void kprint(char* str);
void kprintf(const char *fmt, ...);
void ksprintf(const char *fmt, char* buffer, int buf_len, ...);
void vkprintf(const char *fmt, char* buffer, int buf_len, va_list args);
int sscanf(const char *str, const char *format, ...);