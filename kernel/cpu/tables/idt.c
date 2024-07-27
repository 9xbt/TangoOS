#include <cpu/tables/idt.h>
#include <libs/printf.h>

struct idt_entry idt_entries[256];
struct idtr idt_descriptor;
extern void *idt_int_table[];

void *irq_handlers[256];

const char* isr_errors[32] = {
    "division by zero",
    "debug",
    "non-maskable interrupt",
    "breakpoint",
    "detected overflow",
    "out-of-bounds",
    "invalid opcode",
    "no coprocessor",
    "double fault",
    "coprocessor segment overrun",
    "bad TSS",
    "segment not present",
    "stack fault",
    "general protection fault",
    "page fault",
    "unknown interrupt",
    "coprocessor fault",
    "alignment check",
    "machine check",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved",
    "reserved"
};

void idt_install(void) {
    for (uint16_t i = 0; i < 256; i++) {
        idt_set_entry(i, (uint32_t)idt_int_table[i], 0x08, 0x8E);
    }
    dprintf("idt_install: successfully populated 256 entries\n");

    idt_descriptor = (struct idtr) {
        .size = sizeof(struct idt_entry) * 256 - 1,
        .offset = (uint32_t)idt_entries
    };
    dprintf("idt_install: idt descriptor located at 0x%x\n", (uint32_t)&idt_descriptor);

    asm volatile ("lidt %0" :: "m"(idt_descriptor));
    asm volatile ("sti");
}

void idt_set_entry(uint8_t index, uint32_t base, uint16_t selector, uint8_t type) {
    idt_entries[index].base_low = base & 0xFFFF;
    idt_entries[index].selector = selector;
    idt_entries[index].zero = 0x00;
    idt_entries[index].type = type;
    idt_entries[index].base_high = (base >> 16) & 0xFF;
}

void isr_handler(struct registers r) {
    if (r.int_no == 0xff) {
        return; /* spurious interrupt */ 
    }

    asm volatile ("cli");
    printf("x86 Fault: %s\n", isr_errors[r.int_no]);
    for (;;) asm volatile ("hlt");
}

void irq_handler(struct registers r) {
    void(*handler)(struct registers);
    handler = irq_handlers[r.int_no - 32];

    if (handler != (void*)0)
        handler(r);
}