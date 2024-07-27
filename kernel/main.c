#include <cpu/tables/gdt.h>
#include <drivers/vga.h>
#include <drivers/serial.h>
#include <libs/printf.h>

void _main(unsigned long *a, unsigned long b) {
    vga_clear();
    printf("Hello, world! %x", 123);
}