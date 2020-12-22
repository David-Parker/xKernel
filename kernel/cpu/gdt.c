#include <kernel/cpu/gdt.h>
#include <kernel/mem/phymem.h>

void gdt_init()
{
    global_descriptor_table.limit = (sizeof(gdt_entry_t) * 5) - 1;
    global_descriptor_table.base = (_u32)&gdt_entries;

    _u8 gran = GDT_GRAN_SIZE_ONE_KBYTE | GDT_GRAN_OP_SIZE_32_BIT | GDT_GRAN_SEG_LENGTH;

    gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
    gdt_set_gate(1, 0, PHY_MAX_MEM, GDT_ACCESS_SEG_PRESENT | GDT_ACCESS_PRIV_RING_0 | GDT_ACCESS_DIS_TYPE | GDT_ACCESS_SEG_TYPE_CODE, gran); // Kernel code segment
    gdt_set_gate(2, 0, PHY_MAX_MEM, GDT_ACCESS_SEG_PRESENT | GDT_ACCESS_PRIV_RING_0 | GDT_ACCESS_DIS_TYPE | GDT_ACCESS_SEG_TYPE_DATA, gran); // Kernel data segment
    gdt_set_gate(3, 0, PHY_MAX_MEM, GDT_ACCESS_SEG_PRESENT | GDT_ACCESS_PRIV_RING_3 | GDT_ACCESS_DIS_TYPE | GDT_ACCESS_SEG_TYPE_CODE, gran); // User mode code segment
    gdt_set_gate(4, 0, PHY_MAX_MEM, GDT_ACCESS_SEG_PRESENT | GDT_ACCESS_PRIV_RING_3 | GDT_ACCESS_DIS_TYPE | GDT_ACCESS_SEG_TYPE_DATA, gran); // User mode data segment
}

void gdt_set()
{

}

void gdt_set_gate(_s32 num, _u32 base, _u32 limit, _u8 access, _u8 granularity)
{
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= granularity & 0xF0;
    gdt_entries[num].access      = access;
}