#ifndef __ALLOC_H
#define __ALLOC_H

#include <stddef.h>

void alloc_init();

void *kmalloc(size_t n);
void  kfree(void *ptr);

#endif