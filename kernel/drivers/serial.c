#include <drivers/serial.h>
#include <cpu/io.h>

void serial_putchar(const char c) {
    outb(0xe9, c);
}

void dprintf(const char *str) {
    while (*str){
        serial_putchar(*str++);
    }
}