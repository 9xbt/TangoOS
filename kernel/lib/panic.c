#include <stdarg.h>
#include <drivers/vga.h>
#include <lib/printf.h>

/*
 * panic - panics.
 */
void panic(const char *fmt, ...) {
    vga_puts("panic: ");

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    vga_puts(printf_buf);
    va_end(args);

    vga_puts("\n");

    asm volatile ("cli");
    for (;;) asm volatile ("hlt");
}