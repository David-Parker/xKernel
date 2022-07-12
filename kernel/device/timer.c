#include <timer.h>
#include <ioports.h>
#include <tsc.h>

_u64 timer_ticks = 0;
_u64 tsc_last = 0;

void timer_init()
{
    _u32 freq_divisor = 1193180 / TIMER_HZ;

    write_port_byte(PORT_PIT_REG_CTRL, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    _u8 l = (_u8)(freq_divisor & 0xFF);
    _u8 h = (_u8)((freq_divisor>>8) & 0xFF);

    // Send the frequency divisor.
    write_port_byte(PORT_PIT_REG_DATA, l);
    write_port_byte(PORT_PIT_REG_DATA, h);

    tsc_last = read_tsc();
}

void irq_handle_timer(registers_t* irq)
{
    timer_ticks++;
    tsc_last = read_tsc();
}