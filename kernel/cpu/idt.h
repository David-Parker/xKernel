#pragma once

#include <kernel/util/stddef.h>

// Interrupt Descriptor Table

#define SEG_KERNEL_CS 0x08
#define IDT_ENTRIES 256

typedef struct {
    _u16 low_offset; /* Lower 16 bits of handler function address */
    _u16 sel; /* Kernel segment selector */
    _u8 always0;
    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
    _u8 flags; 
    _u16 high_offset; /* Higher 16 bits of handler function address */
} __attribute__((packed)) idt_gate_t ;

typedef struct {
    _u16 limit;
    _u32 base;
} __attribute__((packed)) idt_register_t;

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void idt_set_gate(int n, _u32 handler);
void idt_set();