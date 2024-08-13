#ifndef __RCC_HPP__
#define __RCC_HPP__

#include "stm32f1xx.h"

#ifndef HSE
#define HSE 8000000
#endif // !HSE

#ifndef HSI
#define HSI 8000000
#endif // !HSI

struct Clocks
{
    uint32_t hpre;
    uint32_t ppre1;
    uint32_t ppre2;
    uint32_t timmul1;
    uint32_t timmul2;
    uint32_t adcpre;
    uint32_t sysclk;
    uint32_t hclk;
    uint32_t pclk1;
    uint32_t pclk2;
    uint32_t timclk1;
    uint32_t timclk2;
    uint32_t adcclk;
};

class Rcc
{
private:
    static constexpr uint32_t hpre_table[] = {2, 4, 8, 16, 64, 128, 256, 512};
    static constexpr uint32_t ppre1_table[] = {2, 4, 8, 16};
    static constexpr uint32_t ppre2_table[] = {2, 4, 8, 16};
    static constexpr uint32_t adcpres[] = {2, 4, 6, 8};
    static constexpr uint32_t pllmuls[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 16};
    static Clocks clocks;

    static uint32_t get_sysclk_from_pllclk(void)
    {
        uint32_t pllclk;
        uint32_t pllmul_bits = READ_BIT(RCC->CFGR, RCC_CFGR_PLLMULL) >> RCC_CFGR_PLLMULL_Pos;
        uint32_t pllmul = pllmuls[pllmul_bits];

        if (READ_BIT(RCC->CFGR, RCC_CFGR_PLLSRC) == 0)
        {
            pllclk = HSI / 2 * pllmul;
        }
        else if (READ_BIT(RCC->CFGR, RCC_CFGR_PLLXTPRE) == 0)
        {
            pllclk = HSE * pllmul;
        }
        else
        {
            pllclk = HSE / 2 * pllmul;
        }

        return pllclk;
    }

public:
    static void update_clocks(void)
    {
        uint32_t hpre_bits = READ_BIT(RCC->CFGR, RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos;
        uint32_t ppre1_bits = READ_BIT(RCC->CFGR, RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos;
        uint32_t ppre2_bits = READ_BIT(RCC->CFGR, RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos;
        uint32_t adcpre_bits = READ_BIT(RCC->CFGR, RCC_CFGR_ADCPRE) >> RCC_CFGR_ADCPRE_Pos;
        uint32_t sws_bits = READ_BIT(RCC->CFGR, RCC_CFGR_SWS);

        clocks.hpre = READ_BIT(hpre_bits, 1 << 3) ? hpre_table[READ_BIT(hpre_bits, 0x7)] : 1;
        clocks.ppre1 = READ_BIT(ppre1_bits, 1 << 2) ? ppre1_table[READ_BIT(ppre1_bits, 0x3)] : 1;
        clocks.ppre2 = READ_BIT(ppre2_bits, 1 << 2) ? ppre2_table[READ_BIT(ppre2_bits, 0x3)] : 1;
        clocks.timmul1 = (clocks.ppre1 == 1) ? 1 : 2;
        clocks.timmul2 = (clocks.ppre2 == 1) ? 1 : 2;
        clocks.adcpre = adcpres[adcpre_bits];

        switch (sws_bits)
        {
        case RCC_CFGR_SWS_HSI:
            clocks.sysclk = HSI;
            break;
        case RCC_CFGR_SWS_HSE:
            clocks.sysclk = HSE;
            break;
        case RCC_CFGR_SWS_PLL:
            clocks.sysclk = get_sysclk_from_pllclk();
            break;
        default:
            break;
        }

        clocks.hclk = clocks.sysclk / clocks.hpre;
        clocks.pclk1 = clocks.sysclk / clocks.ppre1;
        clocks.pclk2 = clocks.sysclk / clocks.ppre2;
        clocks.timclk1 = clocks.pclk1 * clocks.timmul1;
        clocks.timclk2 = clocks.pclk2 * clocks.timmul2;
        clocks.adcclk = clocks.hclk / clocks.adcpre;
    }

    static const Clocks &get_clocks(void) { return clocks; }
};

inline Clocks Rcc::clocks;

#endif // !__RCC_HPP__
