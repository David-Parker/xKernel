#pragma once

#define PORT_VGA_CURSOR_QUERY 0x3d4
#define PORT_VGA_CURSOR_RES 0x3d5

unsigned char read_port_byte (unsigned short port);
void write_port_byte (unsigned short port, unsigned char data);
unsigned short read_port_word (unsigned short port);
void write_port_word (unsigned short port, unsigned short data);