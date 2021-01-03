#pragma once
#include <stddef.h>
#include <ds.h>

typedef struct tty_line
{
    char* str;
    bool read_only;
} tty_line_t;

typedef struct tty
{
    int buf_lines;
    int window_len;
    int line_cols;
    ring_buffer_t* lines;
    ring_buffer_t* r_lines;
} tty_t;

// Temp instance until we setup the filesystem
tty_t tty_default;

void tty_init(tty_t* tty, int buf_lines, int window_len, int line_cols);
void tty_putc(tty_t* tty, char c);
void tty_popc(tty_t* tty);
void tty_clear(tty_t* tty);
void tty_scroll(tty_t* tty, int n);

static tty_line_t* create_line(int str_len);
static void reset_reader(tty_t* tty);