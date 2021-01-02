#include <iolib.h>
#include <console.h>
#include <string.h>

const char charMapLower[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
const char charMapUpper[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

void reverse(char str[], int length)
{
    char* b,*e;
    b = str;
    e = str+length-1;

    while (e > b)
    {
        // Swap
        char tmp = *b;
        *b++ = *e;
        *e-- = tmp;
    }
}

char* safe_copy_str(char *destination, const char *source, char *end)
{
    while (*source != '\0' && destination != end)
    {
        *destination++ = *source++;
    }

    return destination;
}

char* safe_copy_char(char* destination, const char c, char* end)
{
    if (destination < end)
    {
        *destination++ = c;
    }

    return destination;
}

char* itoa(char *destination, char *end, int c, int base)
{
    return ltoa(destination, end, c, base);
}

char* itoa_unsigned(char *destination, char *end, unsigned int c, int base)
{
    return ltoa_unsigned(destination, end, c, base);
}

char* ltoa(char* destination, char* end, long c, int base)
{
    return lltoa(destination, end, c, base);
}

char* ltoa_unsigned(char* destination, char* end, unsigned long c, int base)
{
    return lltoa_unsigned(destination, end, c, base);
}

char* lltoa(char* destination, char* end, long long c, int base)
{
    bool neg = false;
    int bufferIndex = 0;
    char buffer[64];
    long long val = c;

    if (base <= 16)
    {
        if (val == 0)
        {
            char zero[] = "0";
            return safe_copy_str(destination, zero, end);
        }
        else if (val < 0)
        {
            neg = true;
            buffer[bufferIndex++] = '-';
        }

        while (val != 0)
        {
            int index = 0;

            if (neg)
                index = ~(val % base) + 1;
            else
                index = val % base;

            char digit = charMapLower[index];
            buffer[bufferIndex++] = digit;
            val /= base;
        }

        buffer[bufferIndex] = '\0';

        if (neg)
            reverse(buffer + 1, bufferIndex - 1);
        else
            reverse(buffer, bufferIndex);

        return safe_copy_str(destination, buffer, end);
    }
    else
    {
        return NULL;
    }
}

char* lltoa_unsigned(char* destination, char* end, unsigned long long c, int base)
{
    int bufferIndex = 0;
    char buffer[64];

    if (base <= 16)
    {
        if (c == 0)
        {
            char zero[] = "0";
            return safe_copy_str(destination, zero, end);
        }

        while (c != 0)
        {
            char digit = charMapLower[c % base];
            buffer[bufferIndex++] = digit;
            c /= base;
        }

        buffer[bufferIndex] = '\0';

        reverse(buffer, bufferIndex);

        return safe_copy_str(destination, buffer, end);
    }
    else
    {
        return NULL;
    }
}

void kprint(char* str)
{
    if (str == NULL)
    {
        return;
    }
    
    char c;
    
    while ((c = *str++) != 0)
    {
        console_putc(c);
    }
}

void kprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buffer[PRINTF_BUFFER_MAX] = {0};
    vkprintf(fmt, buffer, PRINTF_BUFFER_MAX, args);
    va_end(args);
    kprint(buffer);
}

void ksprintf(const char *fmt, char* buffer, int buf_len, ...)
{
    va_list args;
    va_start(args, buf_len);
    vkprintf(fmt, buffer, buf_len, args);
    va_end(args);
}

void vkprintf(const char *fmt, char* buffer, int buf_len, va_list args)
{
    char* pBuf = buffer;
    char* pEndBuf = &buffer[buf_len - 1];

    while (*fmt != '\0')
    {
        // Search for control characters, else output character immediately.
        if (*fmt == '%')
        {
            ++fmt;

            if (*fmt == '\0')
            {
                continue;
            }
            else if (*fmt == 'd' || *fmt == 'i')
            {
                long c = va_arg(args, int);
                pBuf = itoa(pBuf, pEndBuf, c, 10);
            }
            else if (*fmt == 'u')
            {
                unsigned int c = va_arg(args, unsigned int);
                pBuf = itoa_unsigned(pBuf, pEndBuf, c, 10);
            }
            else if (*fmt == 'x')
            {
                int c = va_arg(args, int);
                pBuf = itoa(pBuf, pEndBuf, c, 16);
            }
            else if (*fmt == 'b')
            {
                int c = va_arg(args, int);
                pBuf = itoa(pBuf, pEndBuf, c, 2);
            }
            else if (*fmt == 'l')
            {
                ++fmt;

                if (*fmt == '\0')
                {
                    continue;
                }
                else if (*fmt == 'l')
                {
                    ++fmt;

                    if (*fmt == '\0')
                    {
                        continue;
                    }
                    else if (*fmt == 'd')
                    {
                        long long c = va_arg(args, long long);
                        pBuf = lltoa(pBuf, pEndBuf, c, 10);
                    }
                    else if (*fmt == 'u')
                    {
                        unsigned long long c = va_arg(args, unsigned long long);
                        pBuf = lltoa_unsigned(pBuf, pEndBuf, c, 10);
                    }
                }
                else if (*fmt == 'd')
                {
                    long c = va_arg(args, long);
                    pBuf = ltoa(pBuf, pEndBuf, c, 10);
                }
                else if (*fmt == 'u')
                {
                    unsigned long c = va_arg(args, unsigned long);
                    pBuf = ltoa_unsigned(pBuf, pEndBuf, c, 10);
                }
            }
            else if (*fmt == 's')
            {
                const char* s = va_arg(args, char*);
                pBuf = safe_copy_str(pBuf, s, pEndBuf);
            }
            else if (*fmt == '%')
            {
                pBuf = safe_copy_char(pBuf, '%', pEndBuf);
            }
        }
        else
        {
            pBuf = safe_copy_char(pBuf, *fmt, pEndBuf);
        }

        ++fmt;
    }

    int strLen = pBuf - buffer;

    if (strLen == buf_len)
    {
        buffer[buf_len - 1] = '\0';
    }
    else
    {
        buffer[strLen] = '\0';
    }
}

int sscanf(const char *str, const char *format, ...)
{
	const char *start = str;
	va_list args;

    int scanned = 0;
	
	va_start(args, format);
	for ( ; *format != '\0'; format++) {
		if (*format == '%' && format[1] == 'd') {
			int positive;
			int value;
			int *valp;
			
			if (*str == '-') {
				positive = 0;
				str++;
			} else
				positive = 1;
			if (!isdigit(*str))
				break;
			value = 0;
			do {
				value = (value * 10) - (*str - '0');
				str++;
			} while (isdigit(*str));
			if (positive)
				value = -value;
			valp = va_arg(args, int *);
			*valp = value;
            scanned++;
			format++;
		} else if (*format == *str) {
			str++;
		} else
			break;
	}
	va_end(args);
	return scanned;
}