#include "clock.h"

volatile uint32_t tick = 0;
uint32_t system_clock;

void clock_setup(uint32_t PLLMULL)
{
    /* 两个等待状态 */
    SET_BIT(FLASH->ACR, 0b010 << FLASH_ACR_LATENCY_Pos);

    /* HSE 使能 */
    SET_BIT(RCC->CR, RCC_CR_HSEON);
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == RESET);

    /* AHB, APB1, APB2 的分频设置 */
    MODIFY_REG(RCC->CFGR,
               RCC_CFGR_HPRE|RCC_CFGR_PPRE1|RCC_CFGR_PPRE2,
               RCC_CFGR_HPRE_DIV1|RCC_CFGR_PPRE1_DIV2|RCC_CFGR_PPRE2_DIV1);

    /* PLL 设置 */
    SET_BIT(RCC->CFGR, RCC_CFGR_PLLSRC);
    SET_BIT(RCC->CFGR, PLLMULL);
    SET_BIT(RCC->CR, RCC_CR_PLLON);
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) == RESET);

    /* 系统时钟切换为 PLL */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
    while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);

    system_clock = 8000000U * ((PLLMULL >> RCC_CFGR_PLLMULL_Pos) + 2);
}

void systick_setup(uint32_t system_clock)
{
    /* 使用 SYSCLK */
    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);

    /* 重装值设置(产生 1ms 中断) */
    SysTick->LOAD = system_clock / 1000 - 1;

    /* 中断优先级 15 */
    NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

    /* 中断使能 */
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);

    /* 定时器使能 */
    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);
}
