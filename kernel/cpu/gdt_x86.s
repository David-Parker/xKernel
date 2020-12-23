[GLOBAL gdt_switch_to_kernel_mode]

gdt_switch_to_kernel_mode:
   push ebp
   mov ebp, esp
   mov eax, [ebp+8]  ; Get the pointer to the GDT, passed as a parameter.
   lgdt [eax]        ; Load the new GDT pointer

   mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
   mov ds, ax        ; Load all data segment selectors, since we use paging all segments can access entire memory range
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax
   pop ebp
   jmp 0x08:.ld_cs   ; Code segment can only be loaded via far jump
   
.ld_cs:
   ret
