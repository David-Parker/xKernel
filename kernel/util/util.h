#pragma once

#include <kernel/util/stddef.h>

void memcopy(char *source, char *dest, int nbytes);
void mem_set(_u8 *dest, _u8 val, _u32 len);
bool strcmp(char* lhs, char* rhs);
int strlen(char* str);
void halt();
int min(int lhs, int rhs);
int max(int lhs, int rhs);