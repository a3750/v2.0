#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern volatile uint32_t tick;
extern uint32_t system_clock;

void clock_setup(uint32_t PLLMUL);
void systick_setup(uint32_t system_clock);

static inline void delay_ms(uint32_t delay)
{
    for (uint32_t tickstart = tick; tick - tickstart <= delay;);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !__CLOCK_H__