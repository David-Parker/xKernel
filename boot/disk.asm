load_disk:
    ; setup stack frame
    push bp
    mov bp, sp

    ; [bp + 4] = arg, # of sectors to read
    pusha

    mov ah, 0x02 ; read
    mov al, [bp + 4]
    mov cl, 0x02 ; 0x02 is next sector after boot sector
    mov ch, 0x00 ; cylinder 0
    mov dh, 0x00 ; head 0

    int 0x13
    jc disk_error
    cmp al, [bp + 4]
    jne disk_error

    popa
    mov sp, bp
    pop bp
    ret

disk_error:
    push MSG_DISK_ERROR
    call print
    sub sp, 2
    jmp $

MSG_DISK_ERROR: db "Error reading from disk",10,13,0