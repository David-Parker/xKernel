print:
    ; setup stack frame
    push bp
    mov bp, sp

    ; push all registers we will use
    push ax
    push bx
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
    pop bx
    pop ax
    mov sp, bp
    pop bp
    ret