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
    xor ax,ax      ; We want a segment of 0 for DS for this question
    mov ds,ax      ;     Set AX to appropriate segment value for your situation
    mov es,ax      ; In this case we'll default to ES=DS
    mov bx,0x8000  ; Stack segment can be any usable memory

    cli            ; Disable interrupts to circumvent bug on early 8088 CPUs
    mov ss,bx      ; This places it with the top of the stack @ 0x80000.
    mov sp,ax      ; Set SP=0 so the bottom of stack will be @ 0x8FFFF
    sti            ; Re-enable interrupts

    cld            ; Set the direction flag to be positive direction

    push msg
    call print

    jmp $ ; infinite loop

    %include "file.asm"

    msg db "Hello, world!",10,13,0

; padding and magic number
times 510 - ($-$$) db 0
dw 0xaa55 