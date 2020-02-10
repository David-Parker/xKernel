; Custom boot loader
; For more information on how to create a bootable .iso for this boot loader visit
; https://stackoverflow.com/questions/34268518/creating-a-bootable-iso-image-with-custom-bootloader
[BITS 16]
[org 0x7c00]

SECTION .text
global _main, _start

_main:
    jmp _start

_start:
    cli
    mov ax,0
    mov ds,ax
    mov es,ax
    mov bx, 0x8000

    mov ss,bx   ; Stack segment
    mov sp,ax       ; Stack 0x80000 (top) - 0x8FFFF (bottom)
    sti

    cld            ; Set the direction flag to be positive direction

    push MSG_START
    call print
    sub sp, 2 ; arg pop (msg)

    ; [es:bx] = address for loaded disk sector
    mov bx, 0x9000
    push 1 ; load 1 more sector
    call load_disk
    sub sp, 2
    
    push bx
    call print
    sub sp, 2

    jmp $ ; infinite loop

    %include "print.asm"
    %include "disk.asm"

    MSG_START: db "Starting kernel boot.",10,13,0

; padding and magic number
times 510 - ($-$$) db 0
dw 0xaa55 

; sector 2
db "Hello, from another sector!",10,13,0