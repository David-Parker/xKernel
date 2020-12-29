#include <kernel/cpu/intr.h>
#include <kernel/cpu/idt.h>
#include <kernel/cpu/gdt.h>
#include <kernel/lib/iolib.h>
#include <kernel/util/debug.h>
#include <kernel/hw/ioports.h>
#include <kernel/hw/keyboard.h>
#include <kernel/hw/timer.h>

const char* intr_strings[] = 
{
    "Division by zero exception",
    "Debug exception",
    "Non maskable interrupt",
    "Breakpoint exception",
    "Into detected overflow",
    "Out of bounds exception",
    "Invalid opcode exception",
    "No coprocessor exception",
    "Double fault (pushes an error code)",
    "Coprocessor segment overrun",
    "Bad TSS (pushes an error code)",
    "Segment not present (pushes an error code)",
    "Stack fault (pushes an error code)",
    "General protection fault (pushes an error code)",
    "Page fault (pushes an error code)",
    "Unknown interrupt exception",
    "Coprocessor fault",
    "Alignment check exception",
    "Machine check exception",
    "SIMD floating point exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

isr_t intr_handlers[256] = { NULL };

void intr_init()
{
    intr_handlers[IDT_INTR_DIV_ZERO] = isr_handle_exception;
    intr_handlers[IDT_INTR_DEBUG_EX] = isr_handle_exception;
    intr_handlers[IDT_INTR_BREAKPOINT_EX] = isr_handle_exception;
    intr_handlers[IDT_INTR_OVERFLOW] = isr_handle_exception;
    intr_handlers[IDT_INTR_OUT_OF_BOUNDS_EX] = isr_handle_exception;
    intr_handlers[IDT_INTR_INV_OPCODE_EX] = isr_handle_exception;
    intr_handlers[IDT_INTR_NO_COPROCESSOR_EX] = isr_handle_exception;
    intr_handlers[IDT_INTR_SEGMENT_NOT_PRESENT] = isr_handle_exception;
    intr_handlers[IDT_INTR_STACK_FAULT] = isr_handle_exception;
    intr_handlers[IDT_INTR_GEN_PROT_FAULT] = isr_handle_exception;
    intr_handlers[IDT_INTR_COPROCESSOR_FAULT] = isr_handle_exception;
    intr_handlers[IDT_INTR_SIMD_FP_EX] = isr_handle_exception;

    intr_handlers[IDT_IRQ_PIT_SYSTEM_TIMER] = irq_handle_timer;
    intr_handlers[IDT_IRQ_KEYBOARD_CONTROLLER] = irq_handle_keyboard;
}

// Interrupt Handler
void isr_handler(registers_t* intr)
{
    kassert(intr->cs == GDT_ADDR_SEG_KERNEL_CODE || intr->cs == GDT_ADDR_SEG_USER_CODE);

    if (intr_handlers[intr->int_no] != NULL)
    {
        isr_t handler = intr_handlers[intr->int_no];
        handler(intr);
        return;
    }
    else
    {
        kprintf("Unhandled interrupt: %d\n", intr->int_no);
    }
}

void irq_handler(registers_t* intr)
{
   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   if (intr->int_no >= 40)
   {
       // Send reset signal to slave.
       write_port_byte(0xA0, 0x20);
   }

   // Send reset signal to master. (As well as slave, if necessary).
   write_port_byte(0x20, 0x20);

   if (intr_handlers[intr->int_no] != NULL)
   {
       isr_t handler = intr_handlers[intr->int_no];
       handler(intr);
   }
    else
    {
        kprintf("Unhandled interrupt: %d\n", intr->int_no);
    }
}

void isr_handle_exception(registers_t* intr)
{
    // Exception came from kernel mode
    if (intr->cs == GDT_ADDR_SEG_KERNEL_CODE)
    {
        char buff[4096];
        ksprintf("%s [%d] generated from kernel code.\neip: 0x%x cs: 0x%x, eflags: 0x%x, esp: 0x%x, ss: 0x%x\n", buff, 4096, intr_strings[intr->int_no], intr->err_code, intr->eip, intr->cs, intr->eflags, intr->esp, intr->ss);
        kpanic(buff);
    }
    else
    {
       // deliver signal to user process
    }
}

void intr_register_handler(int intr_no, isr_t intr_handler)
{
    kassert(intr_no >= 0 && intr_no <= 255);

    intr_handlers[intr_no] = intr_handler;
}

_u32 intr_get_flags()
{
    _u32 flags;

    __asm__ ("pushfl; popl %0" : "=g" (flags));

    return flags;
}

_u32 intr_disable()
{
    _u32 old_flags = intr_get_flags();

    __asm__ ("cli");

    return old_flags;
}

_u32 intr_enable()
{
    _u32 old_flags = intr_get_flags();

    __asm__ ("sti");

    return old_flags;
}