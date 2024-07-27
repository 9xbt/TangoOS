#include <cpu/tables/gdt.h>
#include <drivers/serial.h>

struct gdt_entry gdt_entries[3];
struct gdtr gdt_descriptor;

void gdt_install(void) {
    gdt_set_entry(0, 0x0000, 0x00000000, 0b00000000, 0b00000000);
    gdt_set_entry(1, 0xFFFF, 0x00000000, 0b10011011, 0b11001111);
    gdt_set_entry(2, 0xFFFF, 0x00000000, 0b10010011, 0b11001111);

    gdt_descriptor = (struct gdtr) {
        .size = sizeof(struct gdt_entry) * 3,
        .offset = (uint32_t)&gdt_entries
    };

    asm volatile ("lgdt %0" :: "m"(gdt_descriptor) : "memory");
    
    serial_puts("gdt_install: initialized GDT");
}

void gdt_set_entry(uint8_t index, uint16_t limit, uint32_t base, uint8_t access, uint8_t gran) {
    gdt_entries[index].limit = limit;
    gdt_entries[index].base_low = base & 0xFFFF;
    gdt_entries[index].base_mid = (base >> 16) & 0xFF;
    gdt_entries[index].access = access;
    gdt_entries[index].gran = gran;
    gdt_entries[index].base_high = (base >> 24) & 0xFF;
}