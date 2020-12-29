#pragma once

#include <kernel/util/stddef.h>

// Deal with interrupts

const char* intr_strings[32];

typedef struct registers
{
   _u32 ds;                  // Data segment selector
   _u32 edi, esi, ebp, useless, ebx, edx, ecx, eax; // Pushed by pusha.
   _u32 int_no, err_code;    // Interrupt number and error code (if applicable)
   _u32 eip, cs, eflags, esp, ss; // Pushed by the processor automatically.
} registers_t;

// Interrupt handler callback
typedef void (*isr_t)(registers_t*);
isr_t intr_handlers[256];

void intr_init();
void isr_handler(registers_t* irq);
void irq_handler(registers_t* regs);
void isr_handle_exception(registers_t* irq);
void intr_register_handler(int intr_no, isr_t intr_handler);
_u32 intr_get_flags();
_u32 intr_disable();
_u32 intr_enable();

// isr stubs
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// irq stubs
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();