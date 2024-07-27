#include <sys/version.h>
#include <drivers/vga.h>
#include <lib/panic.h>
#include <lib/printf.h>
#include <lib/multiboot.h>
#include <cpu/tables/gdt.h>
#include <cpu/tables/idt.h>

void _main(struct multiboot_info_t *mboot_info, uint32_t mboot_magic) {
    vga_clear();
    vga_enable_cursor();

    if (mboot_magic != 0x2BADB002) {
        panic("invalid multiboot header");
    }

    gdt_install();
    idt_install();

    printf("Welcome to tangoOS v%d.%d (%s %s %s %s)!\n\n", __kernel_version_major, __kernel_version_minor, __kernel_name, __kernel_build_date, __kernel_build_time, __kernel_arch);
    vga_puts("\033[0;34mA");
}