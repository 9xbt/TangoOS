#include <lib/printf.h>
#include <lib/panic.h>
#include <lib/multiboot.h>
#include <sys/version.h>
#include <drivers/vga.h>
#include <drivers/pic.h>
#include <cpu/tables/gdt.h>
#include <cpu/tables/idt.h>

#define BLINDMODE

void _main(struct multiboot_info_t *mboot_info, uint32_t mboot_magic) {
    #ifdef BLINDMODE
    vga_puts("\033[107m\033[30m");
    #endif
    vga_clear();
    vga_enable_cursor();

    if (mboot_magic != 0x2BADB002) {
        panic("invalid multiboot header");
    }

    gdt_install();
    idt_install();
    pic_install();

    printf("Welcome to \033[96mtangoOS\033[30m!\n%s %d.%d %s %s %s\n\n",
        __kernel_name, __kernel_version_major,__kernel_version_minor,
        __kernel_build_date, __kernel_build_time, __kernel_arch);
    printf("\033[46mHello world\033[0;30m");
}