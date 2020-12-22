#include <kernel/util/util.h>

// TODO: more efficient to implement with SIMD
void memcopy(char *source, char *dest, int nbytes)
{
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void mem_set(_u8 *dest, _u8 val, _u32 len) {
    _u8 *temp = (_u8 *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

void halt()
{
    __asm__("hlt");
}