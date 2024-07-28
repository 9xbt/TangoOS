#ifndef __PMM_H
#define __PMM_H

#define PAGE_SIZE 4096

#define DIV_CEILING(x, y) (x + (y - 1)) / y
#define ALIGN_UP(x, y) DIV_CEILING(x, y) * y

#include <lib/multiboot.h>

void pmm_install(struct multiboot_info_t *mbd);

#endif