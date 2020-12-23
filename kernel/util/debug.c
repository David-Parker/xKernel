#include <kernel/util/debug.h>
#include <kernel/util/util.h>
#include <kernel/cpu/intr.h>
#include <kernel/lib/iolib.h>

void kpanic(char* message)
{
    static int loops = 0;

    intr_disable();

    loops++;
    if (loops == 1)
    {
        kprintf("Kernel panic invoked, shutting down kernel.\n%s", message);
    }
    if (loops == 2)
    {
        kprintf("Kernel panic recursion.\n%s", message);
    }
    else
    {
       // If we got here, then the print caused an exception. Nothing we can do but shutdown.
    }
    
    halt();
}