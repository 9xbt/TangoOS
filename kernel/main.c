#include <mm/pmm.h>
#include <lib/panic.h>
#include <lib/printf.h>
#include <lib/multiboot.h>
#include <sys/version.h>
#include <drivers/vga.h>
#include <drivers/pic.h>
#include <drivers/kbd.h>
#include <cpu/tables/gdt.h>
#include <cpu/tables/idt.h>
#include <sys/shell.h>

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
    pmm_install(mboot_info);
    kbd_install();

    printf("\nWelcome to \033[96mtangoOS\033[0m!\n%s %d.%d %s %s %s\n\n",
        __kernel_name, __kernel_version_major,__kernel_version_minor,
        __kernel_build_date, __kernel_build_time, __kernel_arch);

    /*int arr[4];
    arr[4] = 10;

    _Bool* boolPtr;
    int value = 188;

    boolPtr = (_Bool*)&value; // Assign a non-zero value to a _Bool pointer

    // Print the value to avoid optimization
    printf("Bool value: %d\n", *boolPtr);

    int* val = NULL;
    int a = *val;*/

    shell_entry();
}