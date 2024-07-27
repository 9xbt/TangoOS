#ifndef __LIBC_H
#define __LIBC_H

#include <stdbool.h>
#include <stdint.h>

bool str_contains_char(const char* s, const char c);
void str_shift_left(char *str, const int shift);
uint8_t char_to_int(const char c);

#endif