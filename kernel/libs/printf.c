#include <stdarg.h>
#include <stdint.h>
#include <drivers/vga.h>

char printf_buf[1024] = {-1};
int printf_ptr = 0;

void parse_num(uint32_t val, uint32_t base) {
    uint32_t n = val / base;
    int r = val % base;
    if (r < 0) {
        r += base;
        n--;
    }
    if (val >= base) {
        parse_num(n, base);
    }
    printf_buf[printf_ptr++] = (r + '0');
}

void parse_hex(uint32_t val) {
    int i = 8;
    while (i-- > 0) {
        printf_buf[printf_ptr++] = "0123456789abcdef"[val >> (i * 4) & 0x0F];
    }
}

void printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf_ptr = 0;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'd':
                    parse_num(va_arg(args, int), 10);
                    break;
                case 'x':
                    parse_hex(va_arg(args, uint32_t));
                    break;
                case 's':
                    char *str = va_arg(args, char *);
                    while (*str) {
                        printf_buf[printf_ptr++] = *str++;
                    }
                    break;
                case 'c':
                    printf_buf[printf_ptr++] = (char)va_arg(args, int);
                    break;
            }
        }
        else {
            printf_buf[printf_ptr++] = *fmt;
        }
        fmt++;
    }

    va_end(args);
    vga_puts(printf_buf);
}