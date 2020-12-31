#pragma once

#include <kernel/util/stddef.h>
#include <kernel/cpu/intr.h>
#include <kernel/lib/ds.h>

/*
 * Keys and buttons
 *
 * Most of the keys/buttons are modeled after USB HUT 1.12
 * (see http://www.usb.org/developers/hidpage).
 * Abbreviations in the comments:
 * AC - Application Control
 * AL - Application Launch Button
 * SC - System Control
 */

#define KEY_PAGE_UP 73
#define KEY_PAGE_DOWN 81
#define KEY_ENTER 28
#define KEY_BACKSPACE 14
#define KEY_LEFT_SHIFT 42
#define KEY_RIGHT_SHIFT 54
#define KEY_CTRL 29
#define KEY_ALT 56
#define KEY_0 11
#define KEY_1 2
#define KEY_2 3
#define KEY_3 4
#define KEY_4 5
#define KEY_5 6
#define KEY_6 7
#define KEY_7 8
#define KEY_8 9
#define KEY_9 10

#define KEY_Q 16
#define KEY_RIGHT_BRACK 27
#define KEY_a 30
#define KEY_APOS 40
#define KEY_z 44
#define KEY_SLASH 53

ring_buffer_t user_input;
char keyboard_map[128];

void keyboard_init();
bool keyboard_is_printable(int keycode);
void irq_handle_keyboard(registers_t *irq);