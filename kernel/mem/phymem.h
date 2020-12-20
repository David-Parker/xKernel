#pragma once
// Physical memory layout https://www.cs.princeton.edu/courses/archive/fall06/cos318/precepts/precept_5_1.pdf

#define PHY_BIOS_DATA 0x0000
#define PHY_KERNEL_CODE 0x1000
#define PHY_VGA_MEM_START 0xb8000
#define PHY_VGA_MEM_END 0xb8FA0
#define PHY_KERNEL_STACK_START 0x90000
#define PHY_PAGE_MEM_START 0x100000
#define PHY_MAX_MEM 0xffffffff