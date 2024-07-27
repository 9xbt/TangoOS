#include <sys/version.h>
#include <drivers/vga.h>
#include <libs/printf.h>
#include <cpu/tables/gdt.h>
#include <cpu/tables/idt.h>

void _main(unsigned long *a, unsigned long b) {
    vga_clear();

    gdt_install();
    idt_install();

    printf("Welcome to tangoOS v%d.%d (%s %s %s %s)!", __kernel_version_major, __kernel_version_minor, __kernel_name, __kernel_build_date, __kernel_build_time, __kernel_arch);
    vga_puts("\033[0;91mA");
}