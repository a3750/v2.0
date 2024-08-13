#ifndef __GPIO_HPP__
#define __GPIO_HPP__

#include "stm32f1xx.h"
#include "bitband.h"

struct ExtiPrBit : public BitOperater
{
    inline ExtiPrBit(uint8_t bitnum) : BitOperater(EXTI->PR, bitnum) {}
    inline bool is_set(void) const { return bitband; }
    inline void clear(void) { bitband = 1; }
};

template <uint32_t Base, uint8_t Pin = 0>
struct Gpio
{
    struct Input
    {
        static void analog(void);
        static void floating(void);
        static void pull_up(void);
        static void pull_down(void);
    };

    struct Output
    {
        static void push_pull(void);
        static void open_drain(void);
        static void alternate_push_pull(void);
        static void alternate_open_drain(void);
    };

    struct GpioIdrBit : public BitOperater
    {
        inline GpioIdrBit(void) : BitOperater((uint32_t)(&instance->IDR), Pin) {}
        inline bool is_set(void) const { return bitband; }
    };

    struct GpioOdrBit : public BitOperater
    {
        inline GpioOdrBit(void) : BitOperater((uint32_t)(&instance->ODR), Pin) {}
        inline bool is_set(void) const { return bitband; }
        inline void set(void) { bitband = 1; }
        inline void clear(void) { bitband = 0; }
        inline uint32_t operator=(uint32_t rhs) { return (bitband = rhs); }
    };

    static constexpr uint32_t base = Base;
    static constexpr uint8_t pin = Pin;
    static GPIO_TypeDef *instance;
    static const GpioIdrBit idr;
    static GpioOdrBit odr;
    static constexpr uint8_t id = (Base - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE);

    static constexpr bool is_valid(void);

    static void enable_clock(void);
    static void disable_clock(void);
    static void clock(bool enable);

    static Input into_input(void);
    static Output into_output(void);
    static Output into_output_10mhz(void);
    static Output into_output_2mhz(void);
    static Output into_output_50mhz(void);

    static void lock(void);

    static void set_high(void);
    static void set_low(void);
    static void set(auto state);
    static void toggle(void);

    static bool is_high(void);
    static bool is_low(void);
    static bool read(void);

    static bool is_out_high(void);
    static bool is_out_low(void);
    static bool read_output(void);

    static ExtiPrBit pr;
    static void enable_interrupt_from_exti(void);
    static void disable_interrupt_from_exti(void);
    static void enable_event_from_exti(void);
    static void disable_event_from_exti(void);
    static void enable_rising_edge_from_exti(void);
    static void disable_rising_edge_from_exti(void);
    static void enable_falling_edge_from_exti(void);
    static void disable_falling_edge_from_exti(void);
    static void make_interrupt_source_from_afio(void);
};

template <typename T>
struct is_specialization_of_gpio
{
    static constexpr bool value = false;
};

template <uint32_t Base, uint8_t Pin>
struct is_specialization_of_gpio<Gpio<Base, Pin>>
{
    static constexpr bool value = Gpio<Base, Pin>::is_valid();
};

#ifdef GPIOA_BASE
template <uint8_t Pin = 0>
using PA = Gpio<GPIOA_BASE, Pin>;

template <uint8_t Pin = 0>
Gpio<GPIOA_BASE, Pin> pa;

#endif // GPIOA_BASE

#ifdef GPIOB_BASE
template <uint8_t Pin = 0>
using PB = Gpio<GPIOB_BASE, Pin>;

template <uint8_t Pin = 0>
Gpio<GPIOB_BASE, Pin> pb;

#endif // GPIOB_BASE

#ifdef GPIOC_BASE
template <uint8_t Pin = 0>
using PC = Gpio<GPIOC_BASE, Pin>;

template <uint8_t Pin = 0>
Gpio<GPIOC_BASE, Pin> pc;

#endif // GPIOC_BASE

#ifdef GPIOD_BASE
template <uint8_t Pin = 0>
using PD = Gpio<GPIOD_BASE, Pin>;

template <uint8_t Pin = 0>
Gpio<GPIOD_BASE, Pin> pd;

#endif // GPIOD_BASE

#ifdef GPIOE_BASE
template <uint8_t Pin = 0>
using PE = Gpio<GPIOE_BASE, Pin>;

template <uint8_t Pin = 0>
Gpio<GPIOE_BASE, Pin> pe;

#endif // GPIOE_BASE

#ifdef GPIOF_BASE
template <uint8_t Pin = 0>
using PF = Gpio<GPIOF_BASE, Pin>;

template <uint8_t Pin = 0>
Gpio<GPIOF_BASE, Pin> pf;

#endif // GPIOF_BASE

#ifdef GPIOG_BASE
template <uint8_t Pin = 0>
using PG = Gpio<GPIOG_BASE, Pin>;

template <uint8_t Pin = 0>
Gpio<GPIOG_BASE, Pin> pg;

#endif // GPIOG_BASE

template <uint32_t Base, uint8_t Pin>
GPIO_TypeDef *Gpio<Base, Pin>::instance = (GPIO_TypeDef *)Base;

/**
 * @brief 输入寄存器 IDR 该位对应的位带别名区的引用
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
const typename Gpio<Base, Pin>::GpioIdrBit Gpio<Base, Pin>::idr;

/**
 * @brief 输出寄存器 ODR 该位对应的位带别名区的引用
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
typename Gpio<Base, Pin>::GpioOdrBit Gpio<Base, Pin>::odr;

/**
 * @brief EXTI->PR 当前位的引用
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
ExtiPrBit Gpio<Base, Pin>::pr(Pin);

/**
 * @brief 时钟使能
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::enable_clock(void)
{
    static_assert(is_valid());

#ifdef GPIOA_BASE
    if constexpr (Base == GPIOA_BASE)
        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);
#endif // GPIOA_BASE

#ifdef GPIOB_BASE
    if constexpr (Base == GPIOB_BASE)
        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
#endif // GPIOB_BASE

#ifdef GPIOC_BASE
    if constexpr (Base == GPIOC_BASE)
        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);
#endif // GPIOC_BASE

#ifdef GPIOD_BASE
    if constexpr (Base == GPIOD_BASE)
        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPDEN);
#endif // GPIOD_BASE

#ifdef GPIOE_BASE
    if constexpr (Base == GPIOE_BASE)
        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPEEN);
#endif // GPIOE_BASE

#ifdef GPIOF_BASE
    if constexpr (Base == GPIOF_BASE)
        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPFEN);
#endif // GPIOF_BASE

#ifdef GPIOG_BASE
    if constexpr (Base == GPIOG_BASE)
        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPGEN);
#endif // GPIOG_BASE
}

/**
 * @brief 时钟禁止
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::disable_clock(void)
{
    static_assert(is_valid());

#ifdef GPIOA_BASE
    if constexpr (Base == GPIOA_BASE)
        CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);
#endif // GPIOA_BASE

#ifdef GPIOB_BASE
    if constexpr (Base == GPIOB_BASE)
        CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
#endif // GPIOB_BASE

#ifdef GPIOC_BASE
    if constexpr (Base == GPIOC_BASE)
        CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);
#endif // GPIOC_BASE

#ifdef GPIOD_BASE
    if constexpr (Base == GPIOD_BASE)
        CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPDEN);
#endif // GPIOD_BASE

#ifdef GPIOE_BASE
    if constexpr (Base == GPIOE_BASE)
        CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPEEN);
#endif // GPIOE_BASE

#ifdef GPIOF_BASE
    if constexpr (Base == GPIOF_BASE)
        CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPFEN);
#endif // GPIOF_BASE

#ifdef GPIOG_BASE
    if constexpr (Base == GPIOG_BASE)
        CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPGEN);
#endif // GPIOG_BASE
}

/**
 * @brief 时钟使能/禁止
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @param enable 
 * @arg true 使能
 * @arg false 禁止
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::clock(bool enable)
{
    enable ? enable_clock() : disable_clock();
}

/**
 * @brief 配置为输入
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @return Gpio<Base, Pin>&
 */
template <uint32_t Base, uint8_t Pin>
inline typename Gpio<Base, Pin>::Input Gpio<Base, Pin>::into_input(void)
{
    static_assert(is_valid());
    if constexpr (Pin < 8)
        CLEAR_BIT(instance->CRL, 0x3 << (4 * Pin));
    else
        CLEAR_BIT(instance->CRH, 0x3 << (4 * (Pin - 8)));
    return Input{};
}

/**
 * @brief 配置为输出(默认 10MHz)
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @return Gpio<Base, Pin>&
 */
template <uint32_t Base, uint8_t Pin>
inline typename Gpio<Base, Pin>::Output Gpio<Base, Pin>::into_output(void)
{
    static_assert(is_valid());
    if constexpr (Pin < 8)
        MODIFY_REG(instance->CRL, 0x3 << (4 * Pin), 0x1 << (4 * Pin));
    else
        MODIFY_REG(instance->CRH, 0x3 << (4 * (Pin - 8)), 0x1 << (4 * (Pin - 8)));
    return Output{};
}

/**
 * @brief 配置为输出(10MHz)
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @return Gpio<Base, Pin>&
 */
template <uint32_t Base, uint8_t Pin>
inline typename Gpio<Base, Pin>::Output Gpio<Base, Pin>::into_output_10mhz(void)
{
    static_assert(is_valid());
    if constexpr (Pin < 8)
        MODIFY_REG(instance->CRL, 0x3 << (4 * Pin), 0x1 << (4 * Pin));
    else
        MODIFY_REG(instance->CRH, 0x3 << (4 * (Pin - 8)), 0x1 << (4 * (Pin - 8)));
    return Output{};
}

/**
 * @brief 配置为输出(2MHz)
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @return Gpio<Base, Pin>&
 */
template <uint32_t Base, uint8_t Pin>
inline typename Gpio<Base, Pin>::Output Gpio<Base, Pin>::into_output_2mhz(void)
{
    static_assert(is_valid());
    if constexpr (Pin < 8)
        MODIFY_REG(instance->CRL, 0x3 << (4 * Pin), 0x2 << (4 * Pin));
    else
        MODIFY_REG(instance->CRH, 0x3 << (4 * (Pin - 8)), 0x2 << (4 * (Pin - 8)));
    return Output{};
}

/**
 * @brief 配置为输出(50MHz)
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @return Gpio<Base, Pin>&
 */
template <uint32_t Base, uint8_t Pin>
inline typename Gpio<Base, Pin>::Output Gpio<Base, Pin>::into_output_50mhz(void)
{
    static_assert(is_valid());
    if constexpr (Pin < 8)
        SET_BIT(instance->CRL, 0x3 << (4 * Pin));
    else
        SET_BIT(instance->CRH, 0x3 << (4 * (Pin - 8)));
    return Output{};
}

/**
 * @brief 模拟输入
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::Input::analog(void)
{
    static_assert(is_valid());
    if constexpr (Pin < 8)
        CLEAR_BIT(instance->CRL, 0xC << (4 * Pin));
    else
        CLEAR_BIT(instance->CRH, 0xC << (4 * (Pin - 8)));
}

/**
 * @brief 浮空输入
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::Input::floating(void)
{
    static_assert(is_valid());
    if constexpr (Pin < 8)
        MODIFY_REG(instance->CRL, 0xC << (4 * Pin), 0x4 << (4 * Pin));
    else
        MODIFY_REG(instance->CRH, 0xC << (4 * (Pin - 8)), 0x4 << (4 * (Pin - 8)));
}

/**
 * @brief 上拉输入
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::Input::pull_up(void)
{
    static_assert(is_valid());
    if constexpr (Pin < 8)
        MODIFY_REG(instance->CRL, 0xC << (4 * Pin), 0x8 << (4 * Pin));
    else
        MODIFY_REG(instance->CRH, 0xC << (4 * (Pin - 8)), 0x8 << (4 * (Pin - 8)));
    odr = 1;
}

/**
 * @brief 下拉输入
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::Input::pull_down(void)
{
    static_assert(is_valid());
    if constexpr (Pin < 8)
        MODIFY_REG(instance->CRL, 0xC << (4 * Pin), 0x8 << (4 * Pin));
    else
        MODIFY_REG(instance->CRH, 0xC << (4 * (Pin - 8)), 0x8 << (4 * (Pin - 8)));
    odr = 0;
}

/**
 * @brief 通用推挽输出
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::Output::push_pull(void)
{
    static_assert(is_valid());
    if constexpr (Pin < 8)
        CLEAR_BIT(instance->CRL, 0xC << (4 * Pin));
    else
        CLEAR_BIT(instance->CRH, 0xC << (4 * (Pin - 8)));
}

/**
 * @brief 通用开漏输出
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::Output::open_drain(void)
{
    static_assert(is_valid());
    if constexpr (Pin < 8)
        MODIFY_REG(instance->CRL, 0xC << (4 * Pin), 0x4 << (4 * Pin));
    else
        MODIFY_REG(instance->CRH, 0xC << (4 * (Pin - 8)), 0x4 << (4 * (Pin - 8)));
}

/**
 * @brief 复用推挽输出
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::Output::alternate_push_pull(void)
{
    static_assert(is_valid());
    if constexpr (Pin < 8)
        MODIFY_REG(instance->CRL, 0xC << (4 * Pin), 0x8 << (4 * Pin));
    else
        MODIFY_REG(instance->CRH, 0xC << (4 * (Pin - 8)), 0x8 << (4 * (Pin - 8)));
}

/**
 * @brief 复用开漏输出
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::Output::alternate_open_drain(void)
{
    static_assert(is_valid());
    if constexpr (Pin < 8)
        SET_BIT(instance->CRL, 0xC << (4 * Pin));
    else
        SET_BIT(instance->CRH, 0xC << (4 * (Pin - 8)));
}

/**
 * @brief 锁定引脚配置
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::lock(void)
{
    volatile uint32_t temp;
    WRITE_REG(instance->LCKR, GPIO_LCKR_LCKK | (1 << Pin));
    WRITE_REG(instance->LCKR, 1 << Pin);
    WRITE_REG(instance->LCKR, GPIO_LCKR_LCKK | (1 << Pin));
    temp = READ_REG(instance->LCKR);
    static_cast<void>(temp);
}

/**
 * @brief 输出高电平
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::set_high(void)
{
    static_assert(is_valid());
    odr = 1;
}

/**
 * @brief 输出低电平
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::set_low(void)
{
    static_assert(is_valid());
    odr = 0;
}

/**
 * @brief 设置输出电平
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @param state 要设置的状态
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::set(auto state)
{
    static_assert(is_valid());
    odr = state ? 1 : 0;
}

/**
 * @brief 翻转电平
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 */
template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::toggle(void)
{
    static_assert(is_valid());
    odr = !odr;
}

/**
 * @brief 输入是否为高电平
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @return true 是高电平
 * @return false 不是高电平
 */
template <uint32_t Base, uint8_t Pin>
inline bool Gpio<Base, Pin>::is_high(void)
{
    static_assert(is_valid());
    return idr == 1;
}

/**
 * @brief 输入是否为低电平
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @return true 是低电平
 * @return false 不是低电平
 */
template <uint32_t Base, uint8_t Pin>
inline bool Gpio<Base, Pin>::is_low(void)
{
    static_assert(is_valid());
    return idr == 0;
}

/**
 * @brief 读取输入电平
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @return true 高电平
 * @return false 低电平
 */
template <uint32_t Base, uint8_t Pin>
inline bool Gpio<Base, Pin>::read(void)
{
    static_assert(is_valid());
    return (bool)idr;
}

/**
 * @brief 输出是否为高电平
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @return true 输出为高电平
 * @return false 输出不为高电平
 */
template <uint32_t Base, uint8_t Pin>
inline bool Gpio<Base, Pin>::is_out_high(void)
{
    static_assert(is_valid());
    return odr == 1;
}

/**
 * @brief 输出是否为低电平
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @return true 输出为低电平
 * @return false 输出不为低电平
 */
template <uint32_t Base, uint8_t Pin>
inline bool Gpio<Base, Pin>::is_out_low(void)
{
    static_assert(is_valid());
    return odr == 0;
}

/**
 * @brief 读取输出电平
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @return true 高电平
 * @return false 低电平
 */
template <uint32_t Base, uint8_t Pin>
inline bool Gpio<Base, Pin>::read_output(void)
{
    static_assert(is_valid());
    return (bool)odr;
}

template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::enable_interrupt_from_exti(void)
{
    static_assert(is_valid());
    SET_BIT(EXTI->IMR, 1 << Pin);
}

template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::disable_interrupt_from_exti(void)
{
    static_assert(is_valid());
    CLEAR_BIT(EXTI->IMR, 1 << Pin);
}

template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::enable_event_from_exti(void)
{
    static_assert(is_valid());
    SET_BIT(EXTI->EMR, 1 << Pin);
}

template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::disable_event_from_exti(void)
{
    static_assert(is_valid());
    CLEAR_BIT(EXTI->EMR, 1 << Pin);
}

template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::enable_rising_edge_from_exti(void)
{
    static_assert(is_valid());
    SET_BIT(EXTI->RTSR, 1 << Pin);
}

template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::disable_rising_edge_from_exti(void)
{
    static_assert(is_valid());
    CLEAR_BIT(EXTI->RTSR, 1 << Pin);
}

template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::enable_falling_edge_from_exti(void)
{
    static_assert(is_valid());
    SET_BIT(EXTI->FTSR, 1 << Pin);
}

template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::disable_falling_edge_from_exti(void)
{
    static_assert(is_valid());
    CLEAR_BIT(EXTI->FTSR, 1 << Pin);
}

template <uint32_t Base, uint8_t Pin>
inline void Gpio<Base, Pin>::make_interrupt_source_from_afio(void)
{
    static_assert(is_valid());
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);
    MODIFY_REG(AFIO->EXTICR[Pin / 4], 0xF << (4 * (Pin % 4)), id << (4 * (Pin % 4)));
}

/**
 * @brief 判断模板参数数否合法
 *
 * @tparam Base GPIO 端口基地址
 * @tparam Pin 引脚(0~15)
 * @return true 合法
 * @return false 非法
 */
template <uint32_t Base, uint8_t Pin>
inline constexpr bool Gpio<Base, Pin>::is_valid(void)
{
    if constexpr (Pin >= 16)
        return false;

#ifdef GPIOA_BASE
    if constexpr (Base == GPIOA_BASE)
        return true;
#endif // GPIOA_BASE

#ifdef GPIOB_BASE
    if constexpr (Base == GPIOB_BASE)
        return true;
#endif // GPIOB_BASE

#ifdef GPIOC_BASE
    if constexpr (Base == GPIOC_BASE)
        return true;
#endif // GPIOC_BASE

#ifdef GPIOD_BASE
    if constexpr (Base == GPIOD_BASE)
        return true;
#endif // GPIOD_BASE

#ifdef GPIOE_BASE
    if constexpr (Base == GPIOE_BASE)
        return true;
#endif // GPIOE_BASE

#ifdef GPIOF_BASE
    if constexpr (Base == GPIOF_BASE)
        return true;
#endif // GPIOF_BASE

#ifdef GPIOG_BASE
    if constexpr (Base == GPIOG_BASE)
        return true;
#endif // GPIOG_BASE

    return false;
}

#endif // !__GPIO_HPP__
