#include <stdint.h>
#include <drivers/vga.h>
#include <cpu/io.h>

uint8_t vga_x = 0;
uint8_t vga_y = 0;
uint8_t vga_color = 0x07;
uint16_t *vga_buffer = (uint16_t *)0xB8000;

void vga_clear(void) {
    int i = 0;
    for (i = 0; i < 80 * 25; i++) {
        vga_buffer[i] = 0x07 << 8;
    }
}

void vga_puts(const char *str) {
    while (*str){
        vga_putchar(*str++);
    }
}

void vga_putchar(const char c) {
    switch (c) {
        case '\n':
            vga_x = 80;
            break;
        case '\b':
            if (vga_x == 0) {
                vga_x = 79;
                vga_y--;
            }
            break;
        case '\t':
            vga_puts("    ");
            break;
        default:
            vga_buffer[vga_y * 80 + vga_x] = (0x07 << 8) | c;
            vga_x++;
            break;
    }

    if (vga_x >= 80) { 
        vga_x = 0;
        vga_y++;

        if (vga_y >= 25) {
            vga_scroll();
        }
    }

    vga_update_cursor();
}

void vga_scroll(void) {
    for (int i = 0; i < 80 * 24 * 2; i += 2) {
        *(uint16_t *)(0xB8000 + i) = *(uint16_t *)(0xB8000 + i + 160);
    }
    for (int i = 80 * 24 * 2; i < 80 * 25 * 2; i += 2) {
        *(uint16_t *)(0xB8000 + i) = vga_color << 8;
    }
}

void vga_enable_cursor(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | 14);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | 15);
}

void vga_disable_cursor(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void vga_update_cursor(void) {
	uint16_t pos = vga_y * 80 + vga_x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, pos & 0xFF);
	outb(0x3D4, 0x0E);
	outb(0x3D5, (pos >> 8) & 0xFF);
}