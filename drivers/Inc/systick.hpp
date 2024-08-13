#ifndef __SYSTICK_HPP__
#define __SYSTICK_HPP__

#include "stm32f1xx.h"
#include "bitband.h"

struct Systick
{
    struct TickintBit : public BitOperater
    {
        TickintBit(void) : BitOperater((uint32_t)(&SysTick->CTRL), SysTick_CTRL_TICKINT_Pos) {}
        void enable(void) { bitband = 1; }
        void disable(void) { bitband = 0; }
        bool is_enabled(void) { return bitband; }
    };

    struct InterruptBits
    {
        TickintBit tickint;
    };

    static InterruptBits interrupt;
    static void reload(uint32_t load) { SysTick->LOAD = load; }
    static void clear_count(void) { SysTick->VAL = 0; }
    static void count(bool enable) { MODIFY_REG(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk, enable ? SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk : 0); }
    static void delay(uint32_t nms);
};

inline Systick::InterruptBits Systick::interrupt;

inline Systick syst;

#endif // !__SYSTICK_HPP__
