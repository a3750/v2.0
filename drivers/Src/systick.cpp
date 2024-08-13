#include "systick.hpp"

/**
 * @brief 不使用中断的延时函数
 * 
 * @param nms 延时心跳数
 */
void Systick::delay(uint32_t nms)
{
    volatile uint32_t temp = SysTick->CTRL;
    static_cast<void>(temp);

    while (nms)
    {
        if (READ_BIT(SysTick->CTRL, SysTick_CTRL_COUNTFLAG_Msk) != 0U)
        {
            nms--;
        }
    }
}
