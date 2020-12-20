#include <kernel/hw/console.h>
#include <kernel/hw/ports.h>
#include <kernel/mem/phymem.h>

void kprint(char* str)
{
	unsigned char query_high_byte = 14;
	unsigned char query_low_byte = 15;

	// Query cursor position (offset from VGA memory start)
	write_port_byte(PORT_VGA_CURSOR_QUERY, query_high_byte);
    int cursor_index = read_port_byte(PORT_VGA_CURSOR_RES);
    cursor_index <<= 8;
    write_port_byte(PORT_VGA_CURSOR_QUERY, query_low_byte);
    cursor_index += read_port_byte(PORT_VGA_CURSOR_RES);

	// VGA uses 2 bytes per cell
    int offset_from_vga = cursor_index * 2;

    char* video_memory = (char*) (PHY_VGA_MEM + offset_from_vga);

    while (*str != 0)
    {
        *(video_memory) = *str++;
		*(video_memory+1) = VGA_RED_ON_WHITE;
        video_memory+=2;
    }
}