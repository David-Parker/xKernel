#include <kernel/cpu/intr.h>
#include <kernel/cpu/idt.h>
#include <kernel/cpu/gdt.h>
#include <kernel/lib/iolib.h>
#include <kernel/util/debug.h>

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

// Interrupt Handler
void isr_handler(registers_t* irq)
{
    kassert(irq->cs == GDT_ADDR_SEG_KERNEL_CODE || irq->cs == GDT_ADDR_SEG_USER_CODE);

    switch (irq->int_no)
    {
        case IDT_IRQ_DIV_ZERO:
        case IDT_IRQ_DEBUG_EX:
        case IDT_IRQ_BREAKPOINT_EX:
        case IDT_IRQ_OVERFLOW:
        case IDT_IRQ_OUT_OF_BOUNDS_EX:
        case IDT_IRQ_INV_OPCODE_EX:
        case IDT_IRQ_NO_COPROCESSOR_EX:
        case IDT_IRQ_SEGMENT_NOT_PRESENT:
        case IDT_IRQ_STACK_FAULT:
        case IDT_IRQ_GEN_PROT_FAULT:
        case IDT_IRQ_COPROCESSOR_FAULT:
        case IDT_IRQ_SIMD_FP_EX:
            isr_handle_exception(irq);
        default:
            return;
    }
}

void isr_handle_exception(registers_t* irq)
{
    // Exception came from kernel mode
    if (irq->cs == GDT_ADDR_SEG_KERNEL_CODE)
    {
        char buff[4096];
        ksprintf("%s generated from kernel code.\neip: 0x%x cs: 0x%x, eflags: 0x%x, esp: 0x%x, ss: 0x%x", buff, 4096, intr_strings[irq->int_no], irq->eip, irq->cs, irq->eflags, irq->esp, irq->ss);
        kpanic(buff);
    }
    else
    {
       // deliver signal to user process
    }
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