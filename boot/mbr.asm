; The Master Boot Record (MBR) sector. The BIOS loads this sector before all else and places it at address
; 0x7c00 to 0x7e00, which is unfortunate because the kernel is loaded at a lower address (0x1000). So when the 
; kernel becomes large enough (55 sectors long) it will run into and override memory of the bootloader.
; To prevent this issue, the MBR will do nothing more than load the real bootloader code (bootloader.asm)
; into address 0x1000 and load the kernel into address 0x1200
LOADER_OFFSET equ 0x1000

[org 0x7c00]
SECTION .text
    mov [BOOT_DRIVE], dl
    cli
    mov ax,0
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov sp,0xff00   ; Stack 0xff00 (256 byte stack) - grows downward.
    sti

    cld         ; Set the direction flag to be positive direction
    
    push MSG_START
    call print_16
    add sp, 2 ; arg pop (msg)

    ; [es:bx] = address for loaded disk sector
    mov bx, LOADER_OFFSET
    push 2
    push 1 ; Most sectors before we run into the stack
    call load_disk
    add sp, 4

    jmp LOADER_OFFSET

    %include "boot/print_16.asm"
    %include "boot/disk.asm"
    %include "boot/gdt.asm"

    BOOT_DRIVE db 0
    MSG_START: db "Starting MBR.",10,13,0

; padding and magic number
times 510 - ($-$$) db 0
dw 0xaa55