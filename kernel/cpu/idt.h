#pragma once

#include <kernel/util/stddef.h>

// Interrupt Descriptor Table
// More on idt: http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html

#define SEG_KERNEL_CS 0x08
#define IDT_ENTRIES 256

#define IDT_INTR_DIV_ZERO 0x00
#define IDT_INTR_DEBUG_EX 0x01
#define IDT_INTR_NMI 0x02
#define IDT_INTR_BREAKPOINT_EX 0x03
#define IDT_INTR_OVERFLOW 0x04
#define IDT_INTR_OUT_OF_BOUNDS_EX 0x05
#define IDT_INTR_INV_OPCODE_EX 0x06
#define IDT_INTR_NO_COPROCESSOR_EX 0x07
#define IDT_INTR_DOUBLE_FAULT 0x08
#define IDT_INTR_COPROCESSOR_OVERRUN 0x09
#define IDT_INTR_BAD_TSS 0x0A
#define IDT_INTR_SEGMENT_NOT_PRESENT 0x0B
#define IDT_INTR_STACK_FAULT 0x0C
#define IDT_INTR_GEN_PROT_FAULT 0x0D
#define IDT_INTR_PAGE_FAULT 0x0E
#define IDT_INTR_UNKOWN_EX 0x0F
#define IDT_INTR_COPROCESSOR_FAULT 0x10
#define IDT_INTR_ALIGN_CHECK_EX 0x11
#define IDT_INTR_MACHINE_CHECK_EX 0x12
#define IDT_INTR_SIMD_FP_EX 0x13

#define IDT_FLAG_SEG_PRESENT 0b10000000
#define IDT_FLAG_PRIV_RING_0 0b00000000
#define IDT_FLAG_PRIV_RING_1 0b00100000
#define IDT_FLAG_PRIV_RING_2 0b01000000
#define IDT_FLAG_PRIV_RING_3 0b01100000
#define IDT_FLAG_ALWAYS_14 0b00001110

typedef struct {
    _u16 base_lo; /* Lower 16 bits of handler function address */
    _u16 sel; /* Kernel segment selector */
    _u8 always0;
    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
    _u8 flags; 
    _u16 base_hi; /* Higher 16 bits of handler function address */
} __attribute__((packed)) idt_entry_t ;

typedef struct {
    _u16 limit;
    _u32 base;
} __attribute__((packed)) idt_register_t;

idt_register_t interrupt_descriptor_table;
idt_entry_t idt_entries[IDT_ENTRIES];

void idt_init();
void idt_set_entry(int n, _u32 base, _u16 selector, _u8 flags);

extern _cdecl void idt_set(_u32 idt_addr);