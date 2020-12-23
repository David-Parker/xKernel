#include <kernel/cpu/idt.h>
#include <kernel/util/util.h>
#include <kernel/cpu/intr.h>

void idt_init()
{
    interrupt_descriptor_table.limit = sizeof(idt_entry_t) * 256 -1;
    interrupt_descriptor_table.base  = (_u32)&idt_entries;

    mem_set((_u8*)&idt_entries, 0, sizeof(idt_entry_t)*256);

    idt_set_entry(IDT_IRQ_DIV_ZERO, (_u32)isr0, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_DEBUG_EX, (_u32)isr1, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_NMI, (_u32)isr2, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_BREAKPOINT_EX, (_u32)isr3, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_OVERFLOW, (_u32)isr4, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_OUT_OF_BOUNDS_EX, (_u32)isr5, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_INV_OPCODE_EX, (_u32)isr6, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_NO_COPROCESSOR_EX, (_u32)isr7, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_DOUBLE_FAULT, (_u32)isr8, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_COPROCESSOR_OVERRUN, (_u32)isr9, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_BAD_TSS, (_u32)isr10, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_SEGMENT_NOT_PRESENT, (_u32)isr11, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_STACK_FAULT, (_u32)isr12, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_GEN_PROT_FAULT, (_u32)isr13, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_PAGE_FAULT, (_u32)isr14, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_UNKOWN_EX, (_u32)isr15, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_COPROCESSOR_FAULT, (_u32)isr16, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_ALIGN_CHECK_EX, (_u32)isr17, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(IDT_IRQ_MACHINE_CHECK_EX, (_u32)isr18, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);

    // Reserved IRQs
    idt_set_entry(19, (_u32)isr19, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(20, (_u32)isr20, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(21, (_u32)isr21, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(22, (_u32)isr22, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(23, (_u32)isr23, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(24, (_u32)isr24, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(25, (_u32)isr25, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(26, (_u32)isr26, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(27, (_u32)isr27, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(28, (_u32)isr28, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(29, (_u32)isr29, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(30, (_u32)isr30, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);
    idt_set_entry(31, (_u32)isr31, 0x08, IDT_FLAG_SEG_PRESENT | IDT_FLAG_PRIV_RING_0 | IDT_FLAG_ALWAYS_14);

    idt_set((_u32)&interrupt_descriptor_table);
}

void idt_set_entry(int n, _u32 base, _u16 selector, _u8 flags)
{
    idt_entries[n].base_lo = base & 0xFFFF;
    idt_entries[n].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[n].sel     = selector;
    idt_entries[n].always0 = 0;
    idt_entries[n].flags   = flags /* | IDT_FLAG_PRIV_RING_3 */;
}