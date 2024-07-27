[BITS 32]

section .multiboot

MAGIC equ 0x1BADB002
FLAGS equ 0x00000000
CHECKSUM equ -(MAGIC + FLAGS)

dd MAGIC
dd FLAGS
dd CHECKSUM

section .text.prologue

global _start
extern _main
_start:
    cli
    mov esp, stack_top

    push eax
    push ebx

    call _main

    cli
hltLoop:
    hlt
    jmp hltLoop

section .bss

align 16
stack_bottom:
resb 4096
stack_top: