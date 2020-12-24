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

bool strcmp(char* lhs, char* rhs)
{
    if (lhs == NULL || rhs == NULL)
    {
        return lhs == rhs;
    }

    while (*lhs != '\0')
    {
        char c = *lhs++;
        char o = *rhs++;

        if (o == '\0')
        {
            return false;
        }
        if (c != o)
        {
            return false;
        }
    }

    return *rhs == '\0';
}

void halt()
{
    __asm__("hlt");
}