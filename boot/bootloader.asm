; Custom boot loader
; For more information on how to create a bootable .iso for this boot loader visit
; https://stackoverflow.com/questions/34268518/creating-a-bootable-iso-image-with-custom-bootloader
[org 0x7c00]
KERNEL_OFFSET equ 0x1000

SECTION .text

    mov [BOOT_DRIVE], dl
    cli
    mov ax,0
    mov ds,ax
    mov es,ax
    mov bx, 0x8000

    mov ss,bx   ; Stack segment
    mov sp,ax   ; Stack 0x80000 (top) - 0x8FFFF (bottom)
    sti

    cld         ; Set the direction flag to be positive direction

    push MSG_START
    call print_16
    sub sp, 2 ; arg pop (msg)

    ; [es:bx] = address for loaded disk sector
    mov bx, KERNEL_OFFSET
    push 1 ; load 1 more sector
    call load_disk
    sub sp, 2

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

; padding and magic number
times 510 - ($-$$) db 0
dw 0xaa55