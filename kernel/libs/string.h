#ifndef __LIBC_H
#define __LIBC_H

#include <stdbool.h>

bool str_contains_char(const char* s, const char c);
void str_shift_left(char *str, int shift);

#endif