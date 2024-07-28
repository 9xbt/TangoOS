#include <stdint.h>
#include <drivers/vga.h>
#include <lib/string.h>
#include <lib/libc.h>
#include <cpu/io.h>
#include <stdint.h>

static uint8_t ansi_to_vga[] = { 0, 4, 2, 6, 1, 5, 3, 7 };

uint8_t vga_x = 0;
uint8_t vga_y = 0;
uint8_t vga_color = 0x07;
uint16_t *vga_buffer = (uint16_t *)0xB8000;

/*
 * vga_clear - clears the VGA console
 */
void vga_clear(void) {
    for (int i = 0; i < 80 * 25; i++)
        vga_buffer[i] = vga_color << 8;
    vga_x = 0;
    vga_y = 0;
    vga_update_cursor();
}

/*
 * ansi_color_parse - parses an ANSI escape sequence color
 */
void ansi_color_parse(const char* code) {
    switch (*code) {
        case '0': vga_color = 0x07; break; /* reset */
        case '3': vga_color = (vga_color & 0xF0) | ansi_to_vga[char_to_int(*(code + 1))]; break; /* foreground */
        case '4': vga_color = (vga_color & 0xF0) | ansi_to_vga[char_to_int(*(code + 1))] << 4; break; /* background */
        case '9': vga_color = (vga_color & 0xF0) | (ansi_to_vga[char_to_int(*(code + 1))] + 0x8); break; /* high intensity foreground */
        case '1': vga_color = ((vga_color & 0xF0) | (ansi_to_vga[char_to_int(*(code + 2))] + 0x8) << 4); break; /* high intensity background */
    }
}

/*
 * vga_puts - prints a string to the VGA console
 * supports: text, ansi escape codes
 */
void vga_puts(const char* str) {
    while (*str) {
        /* simple ANSI implementation (only colors for now) */
        if (*str == '\033' && *(str+1) == '[') {
            int ansi_end;
            char code[16] = {0}; /* a buffer to hold the ANSI code */
            int code_index = 0;

            for (int i = 2; ; i++) {
                if (*(str+i) == 'm') { 
                    ansi_end = i; 
                    break; 
                }
                if (code_index < (int)(sizeof(code) - 1))
                    code[code_index++] = *(str+i);
            }

            code[code_index] = '\0'; /* null-terminate the code string */

            if (str_contains_char(code, ';'))
                str_shift_left(code, 2);

            ansi_color_parse(code);

            str += ansi_end + 1; /* skip the ANSI sequence including 'm' */
        } else {
            vga_putchar(*str++);
        }
    }
}

/*
 * vga_putchar - prints a character to the VGA console
 */
void vga_putchar(const char c) {
    switch (c) {
        case '\n':
            vga_x = 0;
            vga_y++;
            break;
        case '\b':
            if (vga_x == 0) {
                vga_x = 79;
                vga_y--;
                vga_buffer[vga_y * 80 + vga_x] = vga_color << 8;
            } else {
                vga_x--;
                vga_buffer[vga_y * 80 + vga_x] = vga_color << 8;
            }
            break;
        case '\t':
            vga_puts("    ");
            break;
        default:
            vga_buffer[vga_y * 80 + vga_x] = (vga_color << 8) | c;
            vga_x++;
            break;
    }

    if (vga_x >= 80) { 
        vga_x = 0;
        vga_y++;
    }

    if (vga_y >= 25) {
        vga_scroll();
    }

    vga_update_cursor();
}

/*
 * vga_scroll - scrolls the VGA console
 */
void vga_scroll(void) {
    for (int i = 0; i < 80 * 24 * 2; i += 2)
        *(uint16_t *)(0xB8000 + i) = *(uint16_t *)(0xB8000 + i + 160);
    for (int i = 80 * 24 * 2; i < 80 * 25 * 2; i += 2)
        *(uint16_t *)(0xB8000 + i) = vga_color << 8;

    vga_y--;
}

/*
 * vga_enable_cursor - enables the VGA hardware cursor
 */
void vga_enable_cursor(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | 14);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | 15);
}

/*
 * vga_disable_cursor - disables the VGA hardware cursor
 */
void vga_disable_cursor(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

/*
 * vga_update_cursor - update the VGA hardware cursor position
 */
void vga_update_cursor(void) {
	uint16_t pos = vga_y * 80 + vga_x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, pos & 0xFF);
	outb(0x3D4, 0x0E);
	outb(0x3D5, (pos >> 8) & 0xFF);
}