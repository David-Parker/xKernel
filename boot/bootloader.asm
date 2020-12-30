; Custom boot loader
; For more information on how to create a bootable .iso for this boot loader visit
; https://stackoverflow.com/questions/34268518/creating-a-bootable-iso-image-with-custom-bootloader
KERNEL_OFFSET equ 0x1200

[org 0x1000]
SECTION .text
    mov sp,0xff00   ; Stack 0xff00 (256 byte stack) - grows downward.
    cld         ; Set the direction flag to be positive direction
    
    push MSG_START
    call print_16
    add sp, 2 ; arg pop (msg)

    ; [es:bx] = address for loaded disk sector
    mov bx, KERNEL_OFFSET
    push 3
    push 118 ; Most sectors before we run into the stack
    call load_disk
    add sp, 4

    call switch_to_pm

    jmp $ ; infinite loop

    %include "boot/print_16.asm"
    %include "boot/disk.asm"
    %include "boot/gdt.asm"
    %include "boot/print_32.asm"
    %include "boot/protmode.asm"

    BOOT_DRIVE db 0
    MSG_START: db "Starting kernel boot.",10,13,0
    MSG_PROT_MODE db "Loaded 32-bit protected mode", 0

; Main entry after enabling 32-bit protected mode
[BITS 32]
Prot_Main:
    mov ebx, MSG_PROT_MODE
    call print_32 ; Note that this will be written at the top left corner
    call KERNEL_OFFSET
    jmp $

; padding
times 512 - ($-$$) db 0