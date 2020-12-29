#pragma once

#include <kernel/util/stddef.h>
#include <kernel/cpu/intr.h>
#include <kernel/hw/ioports.h>

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

char keyboard_map[128];
void irq_handle_keyboard(registers_t *irq);