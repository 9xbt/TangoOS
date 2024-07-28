#include <drivers/pit.h>
#include <cpu/tables/idt.h>
#include <cpu/io.h>
#include <lib/printf.h>

uint64_t pit_ticks;

void pit_handler(struct registers r) {
    pit_ticks++;
}

void pit_install(void) {
    dprintf("\033[92m * \033[0minitializing timer... ");

    uint16_t div = (uint16_t)(1193180 / 1000);

    outb(0x43, 0x36);
    outb(0x40, (uint8_t)div);
    outb(0x40, (uint8_t)(div >> 8));
    irq_register(0x0, pit_handler);

    dprintf("\033[94m[\033[92mok\033[94m]\033[0m\n");
}

void pit_sleep(uint32_t ms) {
    uint64_t start_ticks = pit_ticks;
    uint64_t end_ticks = start_ticks + ms;

    while (pit_ticks < end_ticks) {
        asm volatile ("hlt");
    }
}