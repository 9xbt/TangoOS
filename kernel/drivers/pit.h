#ifndef __PIT_H
#define __PIT_H

#include <stdint.h>

void pit_install(void);
void pit_sleep(uint32_t ms);

#endif