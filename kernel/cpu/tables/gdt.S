section .text

[extern gdt_descriptor]
[global gdt_load]

; gdt_load - loads the GDT
gdt_load:
    lgdt [gdt_descriptor]

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:gdt2

gdt2:
    ret