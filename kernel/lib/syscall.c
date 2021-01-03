#include <unistd.h>
#include <debug.h>
#include <console.h>
#include <tty.h>

int write(int fd, char c)
{
    kassert(fd == stdout);

    if (fd == stdout)
    {
        tty_t* tty = &tty_default;
        
        tty_putc(tty, c);

        // TODO: sys call should not control console screen
        if (c == '\n')
        {
            console_flush();
        }
    }

    return 1;
}