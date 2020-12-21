#include <kernel/hw/console.h>
#include <kernel/hw/ports.h>
#include <kernel/mem/phymem.h>
#include <kernel/util/util.h>

int get_cursor_index(int row, int col);
PSCREEN get_screen_addr(int row, int col);
int get_cursor_pos();
void set_cursor_pos(int index);
void vga_clamp();
void scroll_screen();

int get_cursor_index(int row, int col)
{
    if (row >= VGA_MAX_ROWS ||
        col >= VGA_MAX_COLS ||
        row < 0 ||
        col < 0)
    {
        return 0;
    }

    return row * VGA_MAX_COLS + col;
}

PSCREEN get_screen_addr(int row, int col)
{
    return (PSCREEN) (PHY_VGA_MEM_START + (get_cursor_index(row, col))*2);
}

int get_cursor_pos()
{
    unsigned char query_high_byte = 14;
	unsigned char query_low_byte = 15;

	// Query cursor position (offset from VGA memory start)
	write_port_byte(PORT_VGA_CTRL, query_high_byte);
    int cursor_index = read_port_byte(PORT_VGA_DATA);
    cursor_index <<= 8;
    write_port_byte(PORT_VGA_CTRL, query_low_byte);
    cursor_index += read_port_byte(PORT_VGA_DATA);

    return cursor_index;
}

void set_cursor_pos(int index)
{
    unsigned char query_low_byte = 14;
	unsigned char query_high_byte = 15;

    write_port_byte(PORT_VGA_CTRL, query_low_byte);
    write_port_byte(PORT_VGA_DATA, (unsigned char)(index >> 8));
    write_port_byte(PORT_VGA_CTRL, query_high_byte);
    write_port_byte(PORT_VGA_DATA, (unsigned char)(index & 0xff));
}

void vga_clamp()
{
    if (vga_col_curr >= VGA_MAX_COLS)
    {
        vga_col_curr = 0;
        vga_row_curr++;
    }
    if (vga_row_curr >= VGA_MAX_ROWS - 1) // Always leave last line blank
    {
        scroll_screen();
        vga_row_curr = VGA_MAX_ROWS - 2;
    }
}

void scroll_screen()
{
    PSCREEN screen = get_screen_addr(vga_row_curr, vga_col_curr);
    PSCREEN src = get_screen_addr(1,0);
    PSCREEN dest = get_screen_addr(0,0);
    memcopy(src, dest, (int)(screen - PHY_VGA_MEM_START));
}

void console_init()
{
    vga_row_curr = 0;
    vga_col_curr = 0;
    set_cursor_pos(0);
    clear_screen();
}

void kprint(char* str)
{
    PSCREEN video_memory;
    char c;

    while ((c = *str++) != 0)
    {
        if (c == '\n')
        {
            vga_row_curr++;
            vga_col_curr = 0;
            vga_clamp();
            continue;
        }

        video_memory = get_screen_addr(vga_row_curr, vga_col_curr);

        *(video_memory) = c;
		*(video_memory+1) = VGA_WHITE_ON_BLACK;

        vga_col_curr++;
        vga_clamp();
    }

    int idx = get_cursor_index(vga_row_curr, vga_col_curr);
    set_cursor_pos(idx);
}

void clear_screen()
{
    PSCREEN screen;

    for (int i = 0; i < VGA_MAX_ROWS; ++i)
    {
        for (int j = 0; j < VGA_MAX_COLS; ++j)
        {
            screen = get_screen_addr(i, j);
            *(screen) = ' ';
            *(screen+1) = VGA_WHITE_ON_BLACK;
        }
    }
}