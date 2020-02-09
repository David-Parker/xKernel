print:
    push bp
    mov bp, sp
    mov ah, 0x0e ; tty mode
    mov bx, [bp + 4]
print_loop:
    mov al, [bx]
    cmp al, 0
    je print_final
    int 0x10
    inc bx
    jmp print_loop
print_final:
    mov sp, bp
    pop bp
    ret