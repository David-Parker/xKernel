#include <kernel/util/util.h>

void * memcpy ( void * destination, const void * source, size_t num )
{
    size_t * plDst = (size_t*) destination;
    size_t const * plSrc = (size_t const *) source;
    _u8 * pcDst = (_u8 *) plDst;
    _u8 const * pcSrc = (_u8 const *) plSrc;

    // Only do multi-byte aligned copies for "larger" copies
    if (num > 32 && !align_4(source) && !align_4(destination))
    {
        while (num >= 4)
        {
            *plDst++ = *plSrc++;
            num -= 4;
        }
    }

    while (num--)
    {
        *pcDst++ = *pcSrc++;
    }

    return destination;
}

void* memset (void * ptr, int value, size_t num ) {
    _u8 *temp = (_u8*)ptr;

    while (num--)
    {
        *temp++ = value;
    }

    return ptr;
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