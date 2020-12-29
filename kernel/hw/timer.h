#pragma once

#include <kernel/util/stddef.h>
#include <kernel/cpu/intr.h>

#define TIMER_HZ 18.222

_u64 timer_ticks;

_u64 ktime_get_ns();
void irq_handle_timer(registers_t* irq);