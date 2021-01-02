#pragma once

#include <stddef.h>
#include <intr.h>

#define TIMER_HZ 50

_u64 timer_ticks;

void timer_init();
_u64 ktime_get_ns();
void irq_handle_timer(registers_t* irq);