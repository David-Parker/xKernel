global _start;
[bits 32]

_start:
    mov ebx, MSG_PROT_MODE
    call print_32 ; Note that this will be written at the top left corner
    [extern kmain] ; Define calling point. Must have same name as kernel.c 'kmain' function
    call kmain ; Calls the C function. The linker will know where it is placed in memory
    jmp $

    %include "boot/print_32.asm"

MSG_PROT_MODE db "Calling kmain", 0