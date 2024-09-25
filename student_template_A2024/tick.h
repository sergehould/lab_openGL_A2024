#ifndef TICK_H
#define	TICK_H
#include <stdint.h>

void tick_init(void);
uint32_t tick_get(void);
uint32_t tick_diff(uint32_t stamp);


#endif

