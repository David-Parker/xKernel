#include <kernel/hw/timer.h>

_u64 timer_ticks = 0;

_u64 ktime_get_ns()
{
    return (_u64)(((double)timer_ticks/TIMER_HZ)*NANOS_PER_SEC);
}

void irq_handle_timer(registers_t* irq)
{
    timer_ticks++;
}