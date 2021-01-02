#include <string.h>

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