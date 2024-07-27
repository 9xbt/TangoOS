#include <drivers/serial.h>
#include <cpu/io.h>

/*
 * serial_putchar - prints a character to the QEMU debugcon console
 */
void serial_putchar(const char c) {
    outb(0xe9, c);
}

/*
 * serial_puts - prints a string to the QEMU debugcon console
 */
void serial_puts(const char *str) {
    while (*str){
        serial_putchar(*str++);
    }
}