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

int strlen(char* str)
{
    int len = 0;

    while (*str != '\0')
    {
        len++;
        str++;
    }

    return len;
}

void halt()
{
    __asm__("hlt");
}

int min(int lhs, int rhs)
{
    if (lhs <= rhs) return lhs;
    else return rhs;
}

int max(int lhs, int rhs)
{
    if (lhs >= rhs) return lhs;
    else return rhs;
}

int modulo(int a, int b) {
  int m = a % b;
  if (m < 0) {
    // m += (b < 0) ? -b : b; // avoid this form: it is UB when b == INT_MIN
    m = (b < 0) ? m - b : m + b;
  }
  return m;
}