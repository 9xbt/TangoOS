#include <cpu/tables/idt.h>
#include <drivers/serial.h>

struct idt_entry idt_entries[256];
struct idtr idt_descriptor;
//extern void *idt_int_table[];

void idt_install(void) {
    for (uint8_t i = 0; i < 256; i++) {
        //idt_set_entry(i, (uint32_t)idt_int_table[i], 0x08, 0x8E);
    }
    dprintf("idt_install: successfully populated 256 entries\n");

}

void idt_set_entry(uint8_t index, uint32_t base, uint16_t selector, uint8_t type) {
    idt_entries[index].base_low = base & 0xFFFF;
    idt_entries[index].selector = selector;
    idt_entries[index].zero = 0x00;
    idt_entries[index].type = type;
    idt_entries[index].base_high = (base >> 24) & 0xFF;
}