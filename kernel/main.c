#include <cpu/tables/gdt.h>
#include <drivers/vga.h>
#include <drivers/serial.h>
#include <libs/printf.h>

void _main(unsigned long *a, unsigned long b) {
    vga_clear();
    printf("Hello, world! %x", 123);
    serial_puts("hmm");
    //serial_puts("\033[1m\033[2m\033[3m\033[4m\033[5m\033[6m\033[7m\033[9m\033[31mHello.\n\033[?25h\v\t\f");
    for (;;) {
        //serial_puts("\a"); // repeat the bell sound over and over
    }
}