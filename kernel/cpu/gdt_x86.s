[GLOBAL gdt_switch_to_kernel_mode]    ; Allows the C code to call gdt_flush().

gdt_switch_to_kernel_mode:
   push ebp
   mov ebp, esp
   mov eax, [ebp+8]  ; Get the pointer to the GDT, passed as a parameter.
   lgdt [eax]        ; Load the new GDT pointer

   mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
   mov ds, ax        ; Load all data segment selectors
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax
   pop ebp
   jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
   ret
