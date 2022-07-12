#include <keyboard.h>
#include <iolib.h>
#include <ioports.h>
#include <console.h>
#include <shell.h>

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

size_t key_buffer[512];
ring_buffer_t user_input;

void keyboard_init()
{
    ring_buffer_init(&user_input, key_buffer, ARRSIZE(key_buffer), ARRSIZE(key_buffer));
}

bool keyboard_is_printable(int keycode)
{
    return
        (keycode >= KEY_1 && keycode <= KEY_0) ||
        (keycode >= KEY_Q && keycode <= KEY_RIGHT_BRACK) ||
        (keycode >= KEY_a && keycode <= KEY_APOS) ||
        (keycode >= KEY_z && keycode <= KEY_SLASH) ||
        (keycode == KEY_SPACE)
        ;
}

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

        // kprintf("%d\n", keycode);

        ring_buffer_push(&user_input, keycode);
    }
}