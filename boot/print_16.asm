; 16-bit real mode print (tty)
print_16:
    ; setup stack frame
    push bp
    mov bp, sp

    ; push all registers we will use
    push ax
    push bx
    mov ah, 0x0e ; tty mode
    mov bx, [bp + 4]

print_16_loop:
    mov al, [bx]
    cmp al, 0
    je print_16_final
    int 0x10
    inc bx
    jmp print_16_loop

print_16_final:
    pop bx
    pop ax
    mov sp, bp
    pop bp
    ret