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

    mov sp,0xf000   ; Stack 0xf000 - grows downward. Will be same stack when kernel loaded.
    sti

    cld         ; Set the direction flag to be positive direction

    ; Load the real boot loader
    mov bx, LOADER_OFFSET
    mov ah, 0x02 ; read
    mov al, 1
    mov cl, 0x02 ; 0x02 is next sector after mbr
    mov ch, 0x00 ; cylinder 0
    mov dh, 0x00 ; head 0

    int 0x13
    jmp LOADER_OFFSET

BOOT_DRIVE db 0

; padding and magic number
times 510 - ($-$$) db 0
dw 0xaa55