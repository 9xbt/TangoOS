#include <drivers/serial.h>
#include <cpu/io.h>

void serial_puts(const char *str) {
    while (*str){
        serial_putchar(*str++);
    }
}

void serial_putchar(const char c) {
    outb(0xe9, c);
}