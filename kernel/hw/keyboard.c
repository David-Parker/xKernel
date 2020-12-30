#include <kernel/hw/keyboard.h>
#include <kernel/lib/iolib.h>
#include <kernel/hw/console.h>

char keyboard_map[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

void irq_handle_keyboard(registers_t* irq)
{
    _u8 status = read_port_byte(PORT_KEYBOARD_REG_CTRL);

    // Buffer non-empty
    if (status & 0x01)
    {
        _s8 keycode = read_port_byte(PORT_KEYBOARD_REG_DATA);

        if (keycode < 0)
        {
            return;
        }

        switch (keycode)
        {
            case KEY_PAGE_UP:
                console_scroll_n(VGA_MAX_ROWS);
                break;
            case KEY_PAGE_DOWN:
                console_scroll_n(-VGA_MAX_ROWS);
                break;
            default:
                console_putc(keyboard_map[keycode]);
                console_flush();
                break;
        }
    }
}