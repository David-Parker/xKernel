#pragma once

#define VGA_MAX_ROWS 25
#define VGA_MAX_COLS 80
#define VGA_WHITE_ON_BLACK 0x0f
#define VGA_RED_ON_WHITE 0xf4

typedef char* PSCREEN;
int vga_row_curr;
int vga_col_curr;

void console_init();
void kprint(char* str);
void clear_screen();