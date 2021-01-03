#include <console.h>
#include <ioports.h>
#include <phymem.h>
#include <malloc.h>
#include <util.h>
#include <debug.h>
#include <string.h>

int get_cursor_index(int row, int col);
screen_ptr_t get_screen_addr(int row, int col);
int get_cursor_pos();
void render_screen();

void clamp_values(int* row, int* col)
{
    if (*col == VGA_MAX_COLS)
    {
        *col = 0;
        *row++;
    }

    *row = *row % VGA_MAX_ROWS;
    *col = *col % VGA_MAX_COLS;
}

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

int get_cursor_row()
{
    return get_cursor_pos() / VGA_MAX_COLS;
}

int get_cursor_col()
{
    return get_cursor_pos() % VGA_MAX_COLS;
}

screen_ptr_t get_screen_addr(int row, int col)
{
    return (screen_ptr_t) (PHY_VGA_MEM_START + (get_cursor_index(row, col))*2);
}

int get_cursor_pos()
{
    unsigned char query_high_byte = 14;
    unsigned char query_low_byte = 15;

    // Query cursor position (offset from VGA memory start)
    write_port_byte(PORT_VGA_REG_CTRL, query_high_byte);
    int cursor_index = read_port_byte(PORT_VGA_REG_DATA);
    cursor_index <<= 8;
    write_port_byte(PORT_VGA_REG_CTRL, query_low_byte);
    cursor_index += read_port_byte(PORT_VGA_REG_DATA);

    return cursor_index;
}

void set_cursor_pos(int index)
{
    unsigned char query_low_byte = 14;
    unsigned char query_high_byte = 15;

    write_port_byte(PORT_VGA_REG_CTRL, query_low_byte);
    write_port_byte(PORT_VGA_REG_DATA, (unsigned char)(index >> 8));
    write_port_byte(PORT_VGA_REG_CTRL, query_high_byte);
    write_port_byte(PORT_VGA_REG_DATA, (unsigned char)(index & 0xff));
}

void console_init(tty_t* tty)
{
    kassert(tty != NULL);

    tty_current = tty;
    vga_console_color = VGA_COLOR_BLACK;
    vga_font_color = VGA_COLOR_WHITE;
    console_redraw();
}

void console_redraw()
{
    screen_ptr_t screen;

    for (int i = 0; i < VGA_MAX_ROWS; ++i)
    {
        for (int j = 0; j < VGA_MAX_COLS; ++j)
        {
            screen = get_screen_addr(i, j);
            *(screen) = ' ';
            *(screen+1) = VGA_CONSOLE_FONT_COLOR(vga_console_color, vga_font_color);
        }
    }

    print_marquee();
}

void console_flush()
{
    console_read_from_tty();
}

void console_reset()
{
    tty_clear(tty_current);
    console_redraw();
    set_cursor_pos(0);
}

void console_read_from_tty()
{
    if (tty_current->r_lines->idx_start == tty_current->r_lines->idx_end)
    {
        return;
    }

    console_redraw();
    screen_ptr_t video_memory;
    int i = tty_current->r_lines->idx_start;
    int row = 0;
    int col = 0;
    int cur_idx = 0;

    while (i != -1)
    {
        tty_line_t* elem = (tty_line_t*)ring_buffer_get(tty_current->r_lines, i);
        char* str = elem->str;

        col = 0;

        while (*str != '\0')
        {
            char c = *str++;

            video_memory = get_screen_addr(row, col);
            *(video_memory) = c;
            *(video_memory+1) = VGA_CONSOLE_FONT_COLOR(vga_console_color, vga_font_color);
            col++;
            clamp_values(&row, &col);
        }

        cur_idx = get_cursor_index(row, col);

        row++;
        clamp_values(&row, &col);

        i = ring_buffer_next(tty_current->r_lines, i);
    }

    set_cursor_pos(cur_idx);
}

void console_simple_print(int row, char* str)
{
    screen_ptr_t video_memory;

    video_memory = get_screen_addr(row, 0);

    while (*str != '\0')
    {
        *(video_memory) = *str++;
        *(video_memory+1) = VGA_CONSOLE_FONT_COLOR(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
        video_memory+=2;
    }
}

void console_set_colors(_u8 console, _u8 font)
{
    vga_console_color = console;
    vga_font_color = font;
    print_marquee();
}

void print_marquee()
{
    console_simple_print(24, "xKernel v0.01                                                                   ");
}