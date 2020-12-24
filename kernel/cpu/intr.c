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
void isr_handler(registers_t* intr)
{
    kassert(intr->cs == GDT_ADDR_SEG_KERNEL_CODE || intr->cs == GDT_ADDR_SEG_USER_CODE);

    switch (intr->int_no)
    {
        case IDT_INTR_DIV_ZERO:
        case IDT_INTR_DEBUG_EX:
        case IDT_INTR_BREAKPOINT_EX:
        case IDT_INTR_OVERFLOW:
        case IDT_INTR_OUT_OF_BOUNDS_EX:
        case IDT_INTR_INV_OPCODE_EX:
        case IDT_INTR_NO_COPROCESSOR_EX:
        case IDT_INTR_SEGMENT_NOT_PRESENT:
        case IDT_INTR_STACK_FAULT:
        case IDT_INTR_GEN_PROT_FAULT:
        case IDT_INTR_COPROCESSOR_FAULT:
        case IDT_INTR_SIMD_FP_EX:
            isr_handle_exception(intr);
        default:
            return;
    }
}

void isr_handle_exception(registers_t* intr)
{
    // Exception came from kernel mode
    if (intr->cs == GDT_ADDR_SEG_KERNEL_CODE)
    {
        char buff[4096];
        ksprintf("%s generated from kernel code.\neip: 0x%x cs: 0x%x, eflags: 0x%x, esp: 0x%x, ss: 0x%x", buff, 4096, intr_strings[intr->int_no], intr->eip, intr->cs, intr->eflags, intr->esp, intr->ss);
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