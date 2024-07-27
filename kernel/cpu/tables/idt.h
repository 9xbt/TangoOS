#ifndef __IDT_H
#define __IDT_H

#include <stdint.h>

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type;
    uint16_t base_high;
} __attribute__((packed));

struct idtr {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));

void idt_install(void);
void idt_set_entry(uint8_t index, uint32_t base, uint16_t selector, uint8_t type);

#endif