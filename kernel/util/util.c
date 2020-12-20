#include <kernel/util/util.h>

void kprint(char* str)
{
    char* video_memory = (char*) 0xb8000;

    while (*str != 0)
    {
        *(video_memory) = *str++;
        video_memory+=2;
    }
}