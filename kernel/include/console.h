#pragma once

#include <stddef.h>
#include <tty.h>

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

#define VGA_CONSOLE_FONT_COLOR(console, font) (console << 4) | font

typedef char* screen_ptr_t;
static _u8 vga_console_color;
static _u8 vga_font_color;
static int vga_row_curr;
static int vga_col_curr;
static tty_t* tty_current;

static void print_marquee();
static void reset_reader();

void console_init(tty_t* tty);
void console_redraw();
void console_reset();
void console_flush();
void console_read_from_tty();
void console_simple_print(int row, char* str);
void console_set_colors(_u8 console, _u8 font);