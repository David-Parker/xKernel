#include <kernel/util/debug.h>
#include <kernel/hw/console.h>

void kpanic(char* message)
{
    static int loops = 0;

    intr_disable();

    loops++;
    if (loops == 1)
    {
        console_clear();
        console_simple_print(0, "Kernel panic invoked, shutting down kernel.");
        console_simple_print(1, message);
    }
    if (loops == 2)
    {
        console_simple_print(0, "Kernel panic recursion.");
    }
    else
    {
       // If we got here, then the print caused an exception. Nothing we can do but shutdown.
    }
    
    halt();
}