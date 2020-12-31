#pragma once

#include <kernel/util/stddef.h>
#include <kernel/lib/ds.h>

// Writes to a character frame buffer while booted in text mode
// More on text mode: https://en.wikipedia.org/wiki/Text_mode

// Colors defined for the lower byte of VGA memory
// First 4 bits = Background Color
// Last 4 bits = Text color
#define VGA_COLOR_BLACK 0x0
#define VGA_COLOR_BLUE 0x1
#define VGA_COLOR_GREEN 0x2
#define VGA_COLOR_CYAN 0x3
#define VGA_COLOR_RED 0x4
#define VGA_COLOR_MAGENTA 0x5
#define VGA_COLOR_BROWN 0x6
#define VGA_COLOR_LIGHT_GREY 0x7
#define VGA_COLOR_DARK_GREY 0x8
#define VGA_COLOR_LIGHT_BLUE 0x9
#define VGA_COLOR_LIGHT_GREEN 0xa
#define VGA_COLOR_LIGHT_CYAN 0xb
#define VGA_COLOR_LIGHT_RED 0xc
#define VGA_COLOR_LIGHT_MAGENTA 0xd
#define VGA_COLOR_LIGHT_BROWN 0xe
#define VGA_COLOR_WHITE 0xf

#define VGA_MAX_ROWS 25
#define VGA_MAX_COLS 80
#define VIDEO_BUFFER_ROWS 4096

#define VIDEO_RING_WINDOW_SIZE (VGA_MAX_ROWS - 1)

#define VGA_CONSOLE_FONT_COLOR(console, font) (console << 4) | font

typedef struct console_line
{
    char str[(VGA_MAX_COLS * 2) + 1];
    bool read_only;
} console_line_t;

typedef char* screen_ptr_t;
_u8 vga_console_color;
_u8 vga_font_color;
int vga_row_curr;
int vga_col_curr;

static void print_marquee();
static void reset_reader();

void console_init();
void console_clear();
void console_flush();
void console_putc(char c);
void console_popc();
void console_simple_print(int row, char* str);
void console_set_colors(_u8 console, _u8 font);
void console_scroll_n(int n);
void console_scroll_up();
void console_scroll_down();