#include "stm32f1xx_it.h"
#include "clock.h"

void SysTick_Handler(void)
{
    tick++;
}