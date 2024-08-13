#ifndef __TIM_HPP__
#define __TIM_HPP__

#include "stm32f1xx.h"
#include "bitband.h"
#include <cstddef>

#define TIM1_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB2ENR)
#ifdef TIM1_BASE
#define TIM1_CLOCK_MSK RCC_APB2ENR_TIM1EN
#else
#define TIM1_CLOCK_MSK (0UL)
#endif // TIM1_BASE

#define TIM2_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB1ENR)
#ifdef TIM2_BASE
#define TIM2_CLOCK_MSK RCC_APB1ENR_TIM2EN
#else
#define TIM2_CLOCK_MSK (0UL)
#endif // TIM2_BASE

#define TIM3_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB1ENR)
#ifdef TIM3_BASE
#define TIM3_CLOCK_MSK RCC_APB1ENR_TIM3EN
#else
#define TIM3_CLOCK_MSK (0UL)
#endif // TIM3_BASE

#define TIM4_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB1ENR)
#ifdef TIM4_BASE
#define TIM4_CLOCK_MSK RCC_APB1ENR_TIM4EN
#else
#define TIM4_CLOCK_MSK (0UL)
#endif // TIM4_BASE

#define TIM5_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB1ENR)
#ifdef TIM5_BASE
#define TIM5_CLOCK_MSK RCC_APB1ENR_TIM5EN
#else
#define TIM5_CLOCK_MSK (0UL)
#endif // TIM5_BASE

#define TIM6_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB1ENR)
#ifdef TIM6_BASE
#define TIM6_CLOCK_MSK RCC_APB1ENR_TIM6EN
#else
#define TIM6_CLOCK_MSK (0UL)
#endif // TIM6_BASE

#define TIM7_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB1ENR)
#ifdef TIM7_BASE
#define TIM7_CLOCK_MSK RCC_APB1ENR_TIM7EN
#else
#define TIM7_CLOCK_MSK (0UL)
#endif // TIM7_BASE

#define TIM8_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB2ENR)
#ifdef TIM8_BASE
#define TIM8_CLOCK_MSK RCC_APB2ENR_TIM8EN
#else
#define TIM8_CLOCK_MSK (0UL)
#endif // TIM8_BASE

#define TIM9_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB2ENR)
#ifdef TIM9_BASE
#define TIM9_CLOCK_MSK RCC_APB2ENR_TIM9EN
#else
#define TIM9_CLOCK_MSK (0UL)
#endif // TIM9_BASE

#define TIM10_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB2ENR)
#ifdef TIM10_BASE
#define TIM10_CLOCK_MSK RCC_APB2ENR_TIM10EN
#else
#define TIM10_CLOCK_MSK (0UL)
#endif // TIM10_BASE

#define TIM11_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB2ENR)
#ifdef TIM11_BASE
#define TIM11_CLOCK_MSK RCC_APB2ENR_TIM11EN
#else
#define TIM11_CLOCK_MSK (0UL)
#endif // TIM11_BASE

#define TIM12_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB1ENR)
#ifdef TIM12_BASE
#define TIM12_CLOCK_MSK RCC_APB1ENR_TIM12EN
#else
#define TIM12_CLOCK_MSK (0UL)
#endif // TIM12_BASE

#define TIM13_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB1ENR)
#ifdef TIM13_BASE
#define TIM13_CLOCK_MSK RCC_APB1ENR_TIM13EN
#else
#define TIM13_CLOCK_MSK (0UL)
#endif // TIM13_BASE

#define TIM14_CLOCK_REG_OFFSET offsetof(RCC_TypeDef, APB1ENR)
#ifdef TIM14_BASE
#define TIM14_CLOCK_MSK RCC_APB1ENR_TIM14EN
#else
#define TIM14_CLOCK_MSK (0UL)
#endif // TIM14_BASE

template <uint32_t Base>
struct Tim
{
    static TIM_TypeDef *instance;
    static constexpr uint32_t get_id(void);
    static constexpr uint32_t id = get_id();
    static constexpr bool is_valid(void) { return id != (uint32_t)-1; }

    static constexpr uint32_t clock_offset_table[] = {TIM1_CLOCK_REG_OFFSET, TIM2_CLOCK_REG_OFFSET, TIM3_CLOCK_REG_OFFSET, TIM4_CLOCK_REG_OFFSET, TIM5_CLOCK_REG_OFFSET, TIM6_CLOCK_REG_OFFSET, TIM7_CLOCK_REG_OFFSET, TIM8_CLOCK_REG_OFFSET, TIM9_CLOCK_REG_OFFSET, TIM10_CLOCK_REG_OFFSET, TIM11_CLOCK_REG_OFFSET, TIM12_CLOCK_REG_OFFSET, TIM13_CLOCK_REG_OFFSET, TIM14_CLOCK_REG_OFFSET};
    static constexpr uint32_t clock_msk_table[] = {TIM1_CLOCK_MSK, TIM2_CLOCK_MSK, TIM3_CLOCK_MSK, TIM4_CLOCK_MSK, TIM5_CLOCK_MSK, TIM6_CLOCK_MSK, TIM7_CLOCK_MSK, TIM8_CLOCK_MSK, TIM9_CLOCK_MSK, TIM10_CLOCK_MSK, TIM11_CLOCK_MSK, TIM12_CLOCK_MSK, TIM13_CLOCK_MSK, TIM14_CLOCK_MSK};

    static void clock(bool enable);
};

template <uint32_t Base>
TIM_TypeDef *Tim<Base>::instance = (TIM_TypeDef *)Base;

template <uint32_t Base>
void Tim<Base>::clock(bool enable)
{
    static_assert(is_valid());
    MODIFY_REG(*(volatile uint32_t *)(RCC_BASE + clock_offset_table[id]),
               clock_msk_table[id],
               enable ? clock_msk_table[id] : 0);
}

template <uint32_t Base>
constexpr uint32_t Tim<Base>::get_id(void)
{
#ifdef TIM1_BASE
    if constexpr (Base == TIM1_BASE)
        return 0;
#endif // TIM1_BASE

#ifdef TIM2_BASE
    if constexpr (Base == TIM2_BASE)
        return 1;
#endif // TIM2_BASE

#ifdef TIM3_BASE
    if constexpr (Base == TIM3_BASE)
        return 2;
#endif // TIM3_BASE

#ifdef TIM4_BASE
    if constexpr (Base == TIM4_BASE)
        return 3;
#endif // TIM4_BASE

#ifdef TIM5_BASE
    if constexpr (Base == TIM5_BASE)
        return 4;
#endif // TIM5_BASE

#ifdef TIM6_BASE
    if constexpr (Base == TIM6_BASE)
        return 5;
#endif // TIM6_BASE

#ifdef TIM7_BASE
    if constexpr (Base == TIM7_BASE)
        return 6;
#endif // TIM7_BASE

#ifdef TIM8_BASE
    if constexpr (Base == TIM8_BASE)
        return 7;
#endif // TIM8_BASE

#ifdef TIM9_BASE
    if constexpr (Base == TIM9_BASE)
        return 8;
#endif // TIM9_BASE

#ifdef TIM10_BASE
    if constexpr (Base == TIM10_BASE)
        return 9;
#endif // TIM10_BASE

#ifdef TIM11_BASE
    if constexpr (Base == TIM11_BASE)
        return 10;
#endif // TIM11_BASE

#ifdef TIM12_BASE
    if constexpr (Base == TIM12_BASE)
        return 11;
#endif // TIM12_BASE

#ifdef TIM13_BASE
    if constexpr (Base == TIM13_BASE)
        return 12;
#endif // TIM13_BASE

#ifdef TIM14_BASE
    if constexpr (Base == TIM14_BASE)
        return 13;
#endif // TIM14_BASE

    return (uint32_t)-1;
}

#endif // !__TIM_HPP__
