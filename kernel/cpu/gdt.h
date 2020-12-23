#pragma once

#include <kernel/util/stddef.h>

// Global Descriptor Table
// More about GDT http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
// and https://stackoverflow.com/questions/23978486/far-jump-in-gdt-in-bootloader
#define GDT_ACCESS_SEG_PRESENT 0b10000000
#define GDT_ACCESS_PRIV_RING_0 0b00000000
#define GDT_ACCESS_PRIV_RING_1 0b00100000
#define GDT_ACCESS_PRIV_RING_2 0b01000000
#define GDT_ACCESS_PRIV_RING_3 0b01100000
#define GDT_ACCESS_DIS_TYPE 0b00010000
#define GDT_ACCESS_SEG_TYPE_CODE 0b00001010
#define GDT_ACCESS_SEG_TYPE_DATA 0b00000010

#define GDT_GRAN_SIZE_ONE_BYTE 0b00000000
#define GDT_GRAN_SIZE_ONE_KBYTE 0b10000000
#define GDT_GRAN_OP_SIZE_16_BIT 0b00000000
#define GDT_GRAN_OP_SIZE_32_BIT 0b01000000
#define GDT_GRAN_SEG_LENGTH 0b00001111

typedef struct
{
   _u16 limit_low;           // The lower 16 bits of the limit.
   _u16 base_low;            // The lower 16 bits of the base.
   _u8  base_middle;         // The next 8 bits of the base.
   _u8  access;              // Access flags, determine what ring this segment can be used in.
   _u8  granularity;
   _u8  base_high;           // The last 8 bits of the base.
} __attribute__((packed)) gdt_entry_t;

typedef struct
{
   _u16 limit;               // The upper 16 bits of all selector limits.
   _u32 base;                // The address of the first gdt_entry_t struct.
}
__attribute__((packed)) gdt_register_t;

gdt_register_t global_descriptor_table;
gdt_entry_t gdt_entries[5];

#define GDT_ADDR_SEG_NULL 0x00
#define GDT_ADDR_SEG_KERNEL_CODE 0x08
#define GDT_ADDR_SEG_KERNEL_DATA 0x10
#define GDT_ADDR_SEG_USER_CODE 0x18
#define GDT_ADDR_SEG_USER_DATA 0x20

void gdt_init();
void gdt_set_entry(_s32 num, _u32 base, _u32 limit, _u8 access, _u8 granularity);

extern _cdecl void gdt_switch_to_kernel_mode(_u32 gdt_addr);
extern _cdecl void gdt_switch_to_user_mode(_u32 gdt_addr);