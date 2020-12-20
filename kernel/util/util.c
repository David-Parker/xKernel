#include <kernel/util/util.h>

// TODO: more efficient to implement with SIMD
void memcopy(char *source, char *dest, int nbytes)
{
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}