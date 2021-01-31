#include <tty.h>
#include <debug.h>
#include <malloc.h>
#include <string.h>
#include <phymem.h>
#include <keyboard.h>

void tty_init(tty_t* tty, int buf_lines, int window_len, int line_cols)
{
    tty->buf_lines = buf_lines;
    tty->window_len = window_len;
    tty->line_cols = line_cols;
    size_t* internal_buffer = (size_t*)kcalloc(buf_lines * sizeof(size_t));
    tty->lines = (ring_buffer_t*)kmalloc(sizeof(ring_buffer_t));
    tty->r_lines = (ring_buffer_t*)kmalloc(sizeof(ring_buffer_t));
    ring_buffer_init(tty->lines, internal_buffer, buf_lines, window_len);
    reset_reader(tty);
}

void tty_putc(tty_t* tty, char c)
{
    // Control characters use negative keycode values
    switch (c)
    {
        case -KEY_BACKSPACE:
            tty_popc(tty);
            return;
        case -KEY_PAGE_UP:
            tty_scroll(tty, tty->window_len);
            return;
        case -KEY_PAGE_DOWN:
            tty_scroll(tty, -(tty->window_len));
            return;
    }

    // Empty ring buffer, create first line
    if (tty->lines->idx_start == tty->lines->idx_end)
    {
        tty_line_t* line = create_line(tty->line_cols);

        kassert((size_t)line >= PHY_KERNEL_HEAP);
        tty_line_t* old_line = (tty_line_t*)ring_buffer_push(tty->lines, (size_t)line);

        free(old_line->str);
        free(old_line);
    }

    tty_line_t* curr_line = (tty_line_t*)ring_buffer_get_last(tty->lines);

    if (curr_line->read_only && c != '\n')
    {
        return;
    }

    kassert(curr_line != NULL);
    kassert((size_t)curr_line >= PHY_KERNEL_HEAP);

    int len = strlen(curr_line->str);

    if (c == '\n' || len == tty->line_cols)
    {
        tty_line_t* line = create_line(tty->line_cols);
        tty_line_t* old_line = (tty_line_t*)ring_buffer_push(tty->lines, (size_t)line);
        
        reset_reader(tty);

        free(old_line->str);
        free(old_line);
    }

    if (c == '\n')
    {
        curr_line->read_only = true;
    }
    else
    {
        curr_line = (tty_line_t*)ring_buffer_get_last(tty->lines);
        kassert((size_t)curr_line >= PHY_KERNEL_HEAP);

        len = strlen(curr_line->str);
        curr_line->str[len] = c;
        reset_reader(tty);
    }
}

void tty_popc(tty_t* tty)
{
    if (tty->lines->idx_start == tty->lines->idx_end)
    {
        return;
    }
    
    tty_line_t* curr_line = (tty_line_t*)ring_buffer_get_last(tty->lines);
    tty_line_t* line_above = (tty_line_t*)ring_buffer_get(tty->lines, tty->lines->idx_end - 2);

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
            free(curr_line->str);
            free(curr_line);
            ring_buffer_pop(tty->lines);
        }
    }
    else
    {
        curr_line->str[len-1] = '\0';
    }

    reset_reader(tty);
}

void tty_clear(tty_t* tty)
{
    ring_buffer_clear(tty->lines);
    reset_reader(tty);
}

void tty_scroll(tty_t* tty, int n)
{
    int s_idx = tty->r_lines->idx_start;
    int e_idx = tty->r_lines->idx_end;
    int max_up = tty->lines->size - tty->lines->ring_len - tty->r_lines->idx_read;
    int max_down = tty->r_lines->idx_read;

    if (n > 0)
    {
        int moves = min(n, max_up);

        if (moves > 0)
        {
            tty->r_lines->idx_start = ring_buffer_idx(tty->lines, tty->r_lines->idx_start - moves);
            tty->r_lines->idx_end = ring_buffer_idx(tty->lines, tty->r_lines->idx_end - moves);
            tty->r_lines->idx_read += moves;
        }
    }
    else if (n < 0)
    {
        n = -n;

        int moves = min(n, max_down);

        if (moves > 0)
        {
            tty->r_lines->idx_start = ring_buffer_idx(tty->lines, tty->r_lines->idx_start + moves);
            tty->r_lines->idx_end = ring_buffer_idx(tty->lines, tty->r_lines->idx_end + moves);
            tty->r_lines->idx_read -= moves;
        }
    }
}

bool tty_read(tty_t* tty, int* in_out_index, tty_line_t** out_elem)
{
    if (tty->r_lines->idx_start == tty->r_lines->idx_end)
    {
        return false;
    }

    int i = *in_out_index;

    if (i == -1)
    {
        return false;
    }

    *out_elem = (tty_line_t*)ring_buffer_get(tty->r_lines, i);

    *in_out_index = ring_buffer_next(tty->r_lines, i);

    return true;
}

tty_line_t* create_line(int str_len)
{
    tty_line_t* line = (tty_line_t*)kmalloc(sizeof(tty_line_t));
    char* str = (char*)kcalloc(str_len + 1);
    line->str = str;
    line->read_only = false;

    return line;
}

void reset_reader(tty_t* tty)
{
    ring_buffer_copy(tty->lines, tty->r_lines);
}