#ifndef __PRINTF_H
#define __PRINTF_H

#include <stdarg.h>

extern char printf_buf[];

int vprintf(const char *fmt, va_list args);
int printf(const char *fmt, ...);
int dprintf(const char *fmt, ...);

#endif