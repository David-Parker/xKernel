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

void console_init()
{
    ring_buffer_init(&video_ring, (size_t*)kcalloc(VIDEO_BUFFER_ROWS * sizeof(size_t)), VIDEO_BUFFER_ROWS, VGA_MAX_ROWS - 1);
    vga_console_color = VGA_COLOR_BLACK;
    vga_font_color = VGA_COLOR_WHITE;
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
            *(screen+1) = VGA_CONSOLE_FONT_COLOR(vga_console_color, vga_font_color);
        }
    }

    ring_buffer_clear(&video_ring);
    set_cursor_pos(0);
    reset_reader();
    print_marquee();
}

void console_flush()
{
    render_screen();
}

void render_screen()
{
    if (video_ring.idx_start == video_ring.idx_end)
    {
        return;
    }

    screen_ptr_t video_memory;
    int i = video_ring_reader.idx_start;
    int row = 0;
    int col = 0;
    int cur_idx = 0;

    while (i != -1)
    {
        console_line_t* elem = (console_line_t*)ring_buffer_get(&video_ring_reader, i);
        char* str = elem->str;

        col = 0;

        while (*str != '\0')
        {
            char c = *str++;

            video_memory = get_screen_addr(row, col);
            *(video_memory) = c;
            *(video_memory+1) = *str++;
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

    set_cursor_pos(cur_idx);
}

void console_putc(char c)
{
    if (video_ring.idx_start == video_ring.idx_end)
    {
        console_line_t* line = (console_line_t*)kcalloc(sizeof(console_line_t));

        kassert((size_t)line >= PHY_KERNEL_HEAP);
        console_line_t* old_line = (console_line_t*)ring_buffer_push(&video_ring, (size_t)line);

        if (old_line != NULL)
            free(old_line);
    }

    console_line_t* curr_line = (console_line_t*)ring_buffer_get_last(&video_ring);

    if (curr_line->read_only && c != '\n')
    {
        return;
    }

    kassert(curr_line != NULL);
    kassert((size_t)curr_line >= PHY_KERNEL_HEAP);

    int len = strlen(curr_line->str) / 2;

    if (c == '\n' || len == VGA_MAX_COLS)
    {
        console_line_t* line = (console_line_t*)kcalloc(sizeof(console_line_t));
        console_line_t* old_line = (console_line_t*)ring_buffer_push(&video_ring, (size_t)line);
        
        // The reset needs to happen before the console flush so that the new line is rendered
        reset_reader();
        console_flush();

        if (old_line != NULL)
            free(old_line);
    }

    if (c == '\n')
    {
        curr_line->read_only = true;
    }
    else
    {
        curr_line = (console_line_t*)ring_buffer_get_last(&video_ring);
        kassert((size_t)curr_line >= PHY_KERNEL_HEAP);

        len = strlen(curr_line->str);
        curr_line->str[len] = c;
        curr_line->str[len + 1] = VGA_CONSOLE_FONT_COLOR(vga_console_color, vga_font_color);
        reset_reader();
    }
}

void console_popc()
{
    if (video_ring.idx_start == video_ring.idx_end)
    {
        return;
    }
    
    console_line_t* curr_line = (console_line_t*)ring_buffer_get_last(&video_ring);
    console_line_t* line_above = (console_line_t*)ring_buffer_get(&video_ring, video_ring.idx_end - 2);

    if (curr_line->read_only)
    {
        return;
    }

    kassert(curr_line != NULL);
    kassert((size_t)curr_line >= PHY_KERNEL_HEAP);

    int len = strlen(curr_line->str);

    if (len == 0)
    {
        if (line_above != NULL && !line_above->read_only)
        {
            free(curr_line);
            ring_buffer_pop(&video_ring);

            int row = get_cursor_row();
            set_cursor_pos(get_cursor_index(row-1, 0));
        }
    }
    else
    {
        int row = get_cursor_row();
        int col = get_cursor_col();
        set_cursor_pos(get_cursor_index(row, col-1));
        curr_line->str[len-2] = '\0';
    }

    reset_reader();
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
    print_marquee();
}

void console_scroll_n(int n)
{
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

            console_line_t* line = (console_line_t*)ring_buffer_get(&video_ring_reader, prev_start);

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

            console_line_t* line = (console_line_t*)ring_buffer_get(&video_ring_reader, next_start);

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

void print_marquee()
{
    console_simple_print(24, "xKernel v0.01                                                                   ");
}

void reset_reader()
{
    ring_buffer_copy(&video_ring, &video_ring_reader);
}