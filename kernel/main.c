#include <cpu/tables/gdt.h>
#include <drivers/vga.h>
#include <drivers/serial.h>

void _main(unsigned long *a, unsigned long b) {
    vga_clear();
    vga_puts("Hello, world!");
    serial_puts("\033[1m\033[2m\033[3m\033[4m\033[5m\033[6m\033[7m\033[9m\033[31mHello.\n\033[?25h");
    for (;;) {
        //serial_puts("\a"); // repeat the bell sound over and over
    }
}