#ifndef __MULTIBOOT_H
#define __MULTIBOOT_H

#include <stdint.h>

struct multiboot_info_t {
    uint32_t flags;
} __attribute__((packed));

#endif