#include <kernel/hw/console.h>
#include <kernel/hw/ioports.h>
#include <kernel/mem/phymem.h>
#include <kernel/mem/malloc.h>
#include <kernel/util/util.h>
#include <kernel/util/debug.h>

int get_cursor_index(int row, int col);
screen_ptr_t get_screen_addr(int row, int col);
int get_cursor_pos();
void render_screen();

ring_buffer_t video_ring;
ring_buffer_t video_ring_reader;

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

void console_init()
{
    ring_buffer_init(&video_ring, (size_t*)kcalloc(VIDEO_BUFFER_ROWS * sizeof(size_t)), VIDEO_BUFFER_ROWS, VGA_MAX_ROWS - 1);
    vga_console_color = VGA_COLOR_BLACK;
    vga_font_color = VGA_COLOR_WHITE;
    vga_row_curr = 0;
    vga_col_curr = 0;
    set_cursor_pos(0);
    console_clear();
}

void console_clear()
{
    screen_ptr_t screen;

    for (int i = 0; i < VGA_MAX_ROWS; ++i)
    {
        for (int j = 0; j < VGA_MAX_COLS; ++j)
        {
            screen = get_screen_addr(i, j);
            *(screen) = ' ';
        }
    }
}

void console_flush()
{
    render_screen();
}

void render_screen()
{
    kassert(video_ring_reader.idx_start != video_ring_reader.idx_end);

    screen_ptr_t video_memory;
    int i = video_ring_reader.idx_start;
    int row = 0;
    int col = 0;
    int cur_idx = 0;

    while (i != -1)
    {
        char* elem = (char*)ring_buffer_get(&video_ring_reader, i);

        col = 0;

        while (*elem != '\0')
        {
            char c = *elem++;

            video_memory = get_screen_addr(row, col);
            *(video_memory) = c;
            *(video_memory+1) = *elem++;
            col++;
        }

        cur_idx = get_cursor_index(row, col);

        while (col < VGA_MAX_COLS)
        {
            video_memory = get_screen_addr(row, col);
            *(video_memory) = ' ';
            *(video_memory+1) = VGA_CONSOLE_FONT_COLOR(vga_console_color, vga_font_color);
            col++;
        }

        row++;

        i = ring_buffer_next(&video_ring_reader, i);
    }

    console_simple_print(24, "xKernel v0.01");

    set_cursor_pos(cur_idx);
}

void console_putc(char c)
{
    if (video_ring.total_push == 0)
    {
        char* line = (char*)kcalloc((VGA_MAX_COLS*2) + 1);
        kassert((size_t)line >= PHY_KERNEL_HEAP);
        char* old_line = (char*)ring_buffer_push(&video_ring, (size_t)line);

        if (old_line != NULL)
            free(old_line);
    }

    char* curr_line = (char*)ring_buffer_get_last(&video_ring);

    kassert(curr_line != NULL);
    kassert((size_t)curr_line >= PHY_KERNEL_HEAP);

    int len = strlen(curr_line) / 2;

    if (c == '\n' || len == VGA_MAX_COLS)
    {
        char* line = (char*)kcalloc((VGA_MAX_COLS*2) + 1);
        char* old_line = (char*)ring_buffer_push(&video_ring, (size_t)line);
        ring_buffer_copy(&video_ring, &video_ring_reader);
        console_flush();

        if (old_line != NULL)
            free(old_line);
    }

    if (c != '\n')
    {
        curr_line = (char*)ring_buffer_get_last(&video_ring);
        kassert((size_t)curr_line >= PHY_KERNEL_HEAP);

        len = strlen(curr_line);
        curr_line[len] = c;
        curr_line[len + 1] = VGA_CONSOLE_FONT_COLOR(vga_console_color, vga_font_color);
    }

    ring_buffer_copy(&video_ring, &video_ring_reader);
}

void console_simple_print(int row, char* str)
{
    screen_ptr_t video_memory;

    video_memory = get_screen_addr(row, 0);

    while (*str != '\0')
    {
        *(video_memory) = *str++;
        *(video_memory+1) = VGA_CONSOLE_FONT_COLOR(vga_console_color, vga_font_color);
        video_memory+=2;
    }
}

void console_set_colors(_u8 console, _u8 font)
{
    vga_console_color = console;
    vga_font_color = font;
}

void console_scroll_n(int n)
{
    ring_buffer_copy(&video_ring, &video_ring_reader);

    if (n > 0)
    {
        for (int i = 0; i < n; ++i)
        {
            int distance = ring_buffer_distance(
            video_ring_reader.idx_start, 
            video_ring.idx_start, 
            video_ring.buf_len);

            if (distance >= (VIDEO_BUFFER_ROWS - VGA_MAX_ROWS))
            {
                break;
            }

            int prev_start = ring_buffer_prev(&video_ring_reader, video_ring_reader.idx_start);
            int prev_end = ring_buffer_prev(&video_ring_reader, video_ring_reader.idx_end);

            char* line = (char*)ring_buffer_get(&video_ring_reader, prev_start);

            if (line != NULL)
            {
                kassert((size_t)line >= PHY_KERNEL_HEAP);
                video_ring_reader.idx_start = prev_start;
                video_ring_reader.idx_end = prev_end;
            }
        }
    }
    else if (n < 0)
    {
        for (int i = n; i < 0; ++i)
        {
            if (video_ring_reader.idx_start >= video_ring.idx_start)
            {
                break;
            }

            int next_start = ring_buffer_next(&video_ring_reader, video_ring_reader.idx_start);
            int next_end = ring_buffer_next(&video_ring_reader, video_ring_reader.idx_end);

            char* line = (char*)ring_buffer_get(&video_ring_reader, next_start);

            if (line != NULL)
            {
                kassert((size_t)line >= PHY_KERNEL_HEAP);
                video_ring_reader.idx_start = next_start;
                video_ring_reader.idx_end = next_end;
            }
        }
    }

    render_screen();
}

void console_scroll_up()
{
    console_scroll_n(1);
}

void console_scroll_down()
{
    console_scroll_n(-1);
}