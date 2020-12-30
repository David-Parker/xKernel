#pragma once

#define PORT_VGA_REG_CTRL 0x3d4
#define PORT_VGA_REG_DATA 0x3d5

#define PORT_PIT_REG_CTRL 0x43
#define PORT_PIT_REG_DATA 0x40

#define PORT_KEYBOARD_REG_CTRL 0x64
#define PORT_KEYBOARD_REG_DATA 0x60

unsigned char read_port_byte (unsigned short port);
void write_port_byte (unsigned short port, unsigned char data);
unsigned short read_port_word (unsigned short port);
void write_port_word (unsigned short port, unsigned short data);