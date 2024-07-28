#include <stddef.h>
#include <drivers/kbd.h>
#include <cpu/tables/idt.h>
#include <lib/printf.h>
#include <stdbool.h>
#include <cpu/io.h>

bool kbd_key_pressed = false;
char kbd_char = '\0';
bool kbd_caps = false;
bool kbd_ctrl = false;
bool kbd_shift = false;

/*
 * kbd_handler - handles PS/2 keyboard events
 */
void kbd_handler(struct registers r) {
    uint8_t key = inb(0x60);
    if (!(key & 0x80)) {
        switch (key) {
            case 0x2a:
                kbd_shift = true;
                break;
            case 0x36:
                kbd_shift = true;
                break;
            case 0x1d:
                kbd_ctrl = true;
                break;
            case 0x3a:
                kbd_caps = !kbd_caps;
                break;
            default:
                kbd_key_pressed = true;
                if (kbd_shift) {
                    kbd_char = kb_map_keys_shift[key];
                } else if (kbd_caps) {
                    kbd_char = kb_map_keys_caps[key];
                } else {
                    kbd_char = kb_map_keys[key];
                }
                break;
        }
    } else {
        switch (key) {
            case 0xaa:
                kbd_shift = false;
                break;
            case 0xb6:
                kbd_shift = false;
                break;
            case 0x9d:
                kbd_ctrl = false;
                break;
        }
    }
}

/*
 * kbd_getchar - gets a character from the PS/2 keyboard.
 */
char kbd_getchar() {
    if (kbd_key_pressed) {
        kbd_key_pressed = false;
        return kbd_char;
    } else {
        return '\0';
    }
}

/*
 * kbd_ngets - gets a string from the PS/2 keyboard.
 */
void kbd_ngets(char* buf, size_t n) {
    memset(buf, 0, n);
    size_t pos = 0;

    for (;;) {
        asm volatile ("hlt");

        switch(kbd_char) {
            case '\0':
                break;

            case '\n':
                printf("\n");
                kbd_char = 0;
                return;

            case '\b':
                if (pos <= 0)
                    break;
            
                printf("%c \b", kbd_char);

                pos--;
                buf[pos] = 0;
                break;

            default:
                if (kbd_ctrl && (kbd_char == 'l' || kbd_char == 'L')) {
                    printf("\033[2J\033[H");
                    
                    memset(buf, 0, n);
                    kbd_char = 0;
                    return;
                }

                if (pos >= n - 1)
                    break;

                printf("%c", kbd_char);

                buf[pos] = kbd_char;
                pos++;
                break;
        }

        kbd_char = 0;
    }
}

/*
 * kbd_install - sets up the PS/2 keyboard driver
 */
void kbd_install(void) {
    irq_register(1, kbd_handler);
    dprintf("kbd_install: initialized PS/2 keyboard\n");
}