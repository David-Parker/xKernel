#pragma once

#include <kernel/util/stddef.h>

void memcopy(char *source, char *dest, int nbytes);
void memset(_u8 *dest, _u8 val, _u32 len);
void halt();