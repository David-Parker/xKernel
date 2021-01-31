#pragma once
// Physical memory layout https://www.cs.princeton.edu/courses/archive/fall06/cos318/precepts/precept_5_1.pdf

#define PHY_BIOS_DATA 0x0000
#define PHY_KERNEL_CODE 0x1200
#define PHY_VGA_MEM_START 0xb8000 // This address is memory-mapped to VGA controller's video memory while in text mode
#define PHY_VGA_MEM_END 0xb8fa0
#define PHY_KERNEL_HEAP 0x100000
#define PHY_KERNEL_STACK 0xc00000
#define PHY_KERNEL_MAX_MEM 0x40000000
#define PHY_MAX_MEM 0xffffffff

#define stack_init() __asm__("mov %0, %%ebp;mov %%ebp, %%esp" : : "i" (PHY_KERNEL_STACK))