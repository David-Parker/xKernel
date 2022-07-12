#pragma once

#include <stddef.h>
#include <intr.h>

#define TIMER_HZ 50

_u64 timer_ticks;
_u64 tsc_last;

void timer_init();
void irq_handle_timer(registers_t* irq);