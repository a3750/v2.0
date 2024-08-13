#ifndef __SERIAL_HPP__
#define __SERIAL_HPP__

#include "bitband.h"
#include "stm32f1xx.h"
#include "fmt.hpp"
#include "rcc.hpp"
#include "dma.hpp"

#define __SERIAL_CLEAR_FLAG__(instance) \
    do                                  \
    {                                   \
        volatile uint32_t tmpreg;       \
        tmpreg = instance->SR;          \
        (void)tmpreg;                   \
        tmpreg = instance->DR;          \
        (void)tmpreg;                   \
    } while (0U)

template <uint32_t Base>
struct Serial
{
    static USART_TypeDef *instance;
    static constexpr uint32_t tx_dma_base_table[] = {DMA1_BASE, DMA1_BASE, DMA1_BASE, DMA2_BASE, -1};
    static constexpr uint8_t tx_dma_channel_table[] = {3, 6, 1, 4, -1};
    static constexpr uint32_t rx_dma_base_table[] = {DMA1_BASE, DMA1_BASE, DMA1_BASE, DMA2_BASE, -1};
    static constexpr uint8_t rx_dma_channel_table[] = {4, 5, 2, 2, -1};

    static constexpr uint32_t get_id(void)
    {
#ifdef USART1_BASE
        if constexpr (Base == USART1_BASE)
            return 0;
#endif // USART1_BASE

#ifdef USART2_BASE
        if constexpr (Base == USART2_BASE)
            return 1;
#endif // USART2_BASE

#ifdef USART3_BASE
        if constexpr (Base == USART3_BASE)
            return 2;
#endif // USART3_BASE

#ifdef UART4_BASE
        if constexpr (Base == UART4_BASE)
            return 3;
#endif // USRT4_BASE

#ifdef UART5_BASE
        if constexpr (Base == UART5_BASE)
            return 4;
#endif // UART5_BASE
    }

    static constexpr bool is_valid(void);

    static constexpr uint32_t id = get_id();

    struct CtsBit : public BitOperater
    {
        inline CtsBit(void) : BitOperater((uint32_t)(&instance->SR), USART_SR_CTS_Pos) {}
        inline bool is_set(void) const { return bitband; }
        inline void clear(void) { bitband = 0; }
    };

    struct LbdBit : public BitOperater
    {
        inline LbdBit(void) : BitOperater((uint32_t)(&instance->SR), USART_SR_LBD_Pos) {}
        inline bool is_set(void) const { return bitband; }
        inline void clear(void) { bitband = 0; }
    };

    struct TxeBit : public BitOperater
    {
        inline TxeBit(void) : BitOperater((uint32_t)(&instance->SR), USART_SR_TXE_Pos) {}
        inline bool is_set(void) const { return bitband; }
    };

    struct TcBit : public BitOperater
    {
        inline TcBit(void) : BitOperater((uint32_t)(&instance->SR), USART_SR_TC_Pos) {}
        inline bool is_set(void) const { return bitband; }
        inline void clear(void) { bitband = 0; }
    };

    struct RxneBit : public BitOperater
    {
        inline RxneBit(void) : BitOperater((uint32_t)(&instance->SR), USART_SR_RXNE_Pos) {}
        inline bool is_set(void) const { return bitband; }
        inline void clear(void) { bitband = 0; }
    };

    struct IdleBit : public BitOperater
    {
        inline IdleBit(void) : BitOperater((uint32_t)(&instance->SR), USART_SR_IDLE_Pos) {}
        inline bool is_set(void) const { return bitband; }
        inline void clear(void) { __SERIAL_CLEAR_FLAG__(instance); }
    };

    struct OreBit : public BitOperater
    {
        inline OreBit(void) : BitOperater((uint32_t)(&instance->SR), USART_SR_ORE_Pos) {}
        inline bool is_set(void) const { return bitband; }
        inline void clear(void) { __SERIAL_CLEAR_FLAG__(instance); }
    };

    struct NeBit : public BitOperater
    {
        inline NeBit(void) : BitOperater((uint32_t)(&instance->SR), USART_SR_NE_Pos) {}
        inline bool is_set(void) const { return bitband; }
        inline void clear(void) { __SERIAL_CLEAR_FLAG__(instance); }
    };

    struct FeBit : public BitOperater
    {
        inline FeBit(void) : BitOperater((uint32_t)(&instance->SR), USART_SR_FE_Pos) {}
        inline bool is_set(void) const { return bitband; }
        inline void clear(void) { __SERIAL_CLEAR_FLAG__(instance); }
    };

    struct PeBit : public BitOperater
    {
        inline PeBit(void) : BitOperater((uint32_t)(&instance->SR), USART_SR_PE_Pos) {}
        inline bool is_set(void) const { return bitband; }
        inline void clear(void) { __SERIAL_CLEAR_FLAG__(instance); }
    };

    struct PeieBit : public BitOperater
    {
        inline PeieBit(void) : BitOperater((uint32_t)(&instance->CR1), USART_CR1_PEIE_Pos) {}
        inline void enable(void) { bitband = 1; }
        inline void disable(void) { bitband = 0; }
        inline bool is_enabled(void) { return bitband; }
    };

    struct TxeieBit : public BitOperater
    {
        inline TxeieBit(void) : BitOperater((uint32_t)(&instance->CR1), USART_CR1_TXEIE_Pos) {}
        inline void enable(void) { bitband = 1; }
        inline void disable(void) { bitband = 0; }
        inline bool is_enabled(void) { return bitband; }
    };

    struct TcieBit : public BitOperater
    {
        inline TcieBit(void) : BitOperater((uint32_t)(&instance->CR1), USART_CR1_TCIE_Pos) {}
        inline void enable(void) { bitband = 1; }
        inline void disable(void) { bitband = 0; }
        inline bool is_enabled(void) { return bitband; }
    };

    struct RxneieBit : public BitOperater
    {
        inline RxneieBit(void) : BitOperater((uint32_t)(&instance->CR1), USART_CR1_RXNEIE_Pos) {}
        inline void enable(void) { bitband = 1; }
        inline void disable(void) { bitband = 0; }
        inline bool is_enabled(void) { return bitband; }
    };

    struct IdleieBit : public BitOperater
    {
        inline IdleieBit(void) : BitOperater((uint32_t)(&instance->CR1), USART_CR1_IDLEIE_Pos) {}
        inline void enable(void) { bitband = 1; }
        inline void disable(void) { bitband = 0; }
        inline bool is_enabled(void) { return bitband; }
    };

    struct FlagBits
    {
        CtsBit cts;
        LbdBit lbd;
        TxeBit txe;
        TcBit tc;
        RxneBit rxne;
        IdleBit idle;
        OreBit ore;
        NeBit ne;
        FeBit fe;
        PeBit pe;
    };

    struct InterruptBits
    {
        PeieBit peie;
        TxeieBit txeie;
        TcieBit tcie;
        RxneieBit rxneie;
        IdleieBit idleie;
    };

    static DmaChannel<tx_dma_base_table[id], tx_dma_channel_table[id]> tx_dma_channel;
    static DmaChannel<rx_dma_base_table[id], rx_dma_channel_table[id]> rx_dma_channel;
    static FlagBits flag;
    static InterruptBits interrupt;

    Serial(void) { static_assert(is_valid()); }

    static void clock(bool enable);
    static void enable_clock(void);
    static void disable_clock(void);

    static void defult(uint32_t baud = 115200);

    static void baudrate(uint32_t val);
    static void stopbits_1(void);
    static void stopbits_0P5(void);
    static void stopbits_2(void);
    static void stopbits_1P5(void);
    static void wordlength_8bits(void);
    static void wordlength_9bits(void);
    static void parity_none(void);
    static void parity_even(void);
    static void parity_odd(void);

    static void enable(void);
    static void disable(void);
    static void transmitter(bool enable);
    static void receiver(bool enable);
    static void request_transmitter_from_dma(bool enable);
    static void request_receiver_from_dma(bool enable);

    static void write_dr(uint32_t val);
    static uint32_t read_dr(void);

    static void putchar(char character);
    static char getchar(void);
    static void transmit(void *ptr, size_t size);
    static void receive(void *ptr, size_t size);

    template <typename... Args>
    static void print(const char *format, const Args &...args);

    template <typename... Args>
    static void println(const char *format, const Args &...args);
};

template <typename T>
struct is_specialization_of_serial
{
    static constexpr bool value = false;
};

template <uint32_t Base>
struct is_specialization_of_serial<Serial<Base>>
{
    static constexpr bool value = Serial<Base>::is_valid();
};

#ifdef USART1_BASE
using Serial1 = Serial<USART1_BASE>;
inline Serial1 serial1;
#endif // USART1_BASE

#ifdef USART2_BASE
using Serial2 = Serial<USART2_BASE>;
inline Serial2 serial2;
#endif // USART2_BASE

#ifdef USART3_BASE
using Serial3 = Serial<USART3_BASE>;
inline Serial3 serial3;
#endif // USART3_BASE

#ifdef UART4_BASE
using Serial4 = Serial<UART4_BASE>;
inline Serial4 serial4;
#endif // USRT4_BASE

#ifdef UART5_BASE
using Serial5 = Serial<UART5_BASE>;
inline Serial5 serial5;
#endif // UART5_BASE

/**
 * @brief 外设端口结构体指针
 * 
 * @tparam Base 
 */
template <uint32_t Base>
USART_TypeDef *Serial<Base>::instance = (USART_TypeDef *)Base;

/**
 * @brief 控制发送的DMA通道
 * 
 * @tparam Base 
 */
template <uint32_t Base>
DmaChannel<Serial<Base>::tx_dma_base_table[Serial<Base>::id], Serial<Base>::tx_dma_channel_table[Serial<Base>::id]> Serial<Base>::tx_dma_channel;

/**
 * @brief 控制接收的DMA通道
 * 
 * @tparam Base 
 */
template <uint32_t Base>
DmaChannel<Serial<Base>::rx_dma_base_table[Serial<Base>::id], Serial<Base>::rx_dma_channel_table[Serial<Base>::id]> Serial<Base>::rx_dma_channel;

/**
 * @brief 包含本外设状态位及其操作的结构体
 * 
 * @tparam Base 
 */
template <uint32_t Base>
typename Serial<Base>::FlagBits Serial<Base>::flag;

/**
 * @brief 包含本外设中断位及其设置的结构体
 * 
 * @tparam Base 
 */
template <uint32_t Base>
typename Serial<Base>::InterruptBits Serial<Base>::interrupt;

/**
 * @brief 时钟使能/禁止
 * 
 * @tparam Base 
 * @param enable 
 * @arg true 使能
 * @arg false 禁止
 */
template <uint32_t Base>
inline void Serial<Base>::clock(bool enable)
{
    static_assert(is_valid());
    enable ? enable_clock() : disable_clock();
}

/**
 * @brief 时钟使能
 * 
 * @tparam Base 
 */
template <uint32_t Base>
inline void Serial<Base>::enable_clock(void)
{
    static_assert(is_valid());

#ifdef USART1_BASE
    if constexpr (Base == USART1_BASE)
        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
#endif // USART1_BASE

#ifdef USART2_BASE
    if constexpr (Base == USART2_BASE)
        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
#endif // USART2_BASE

#ifdef USART3_BASE
    if constexpr (Base == USART3_BASE)
        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);
#endif // USART3_BASE

#ifdef UART4_BASE
    if constexpr (Base == UART4_BASE)
        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART4EN);
#endif // USRT4_BASE

#ifdef UART5_BASE
    if constexpr (Base == UART5_BASE)
        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART5EN);
#endif // UART5_BASE
}

/**
 * @brief 时钟禁止
 * 
 * @tparam Base 
 */
template <uint32_t Base>
inline void Serial<Base>::disable_clock(void)
{
    static_assert(is_valid());

#ifdef USART1_BASE
    if constexpr (Base == USART1_BASE)
        CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
#endif // USART1_BASE

#ifdef USART2_BASE
    if constexpr (Base == USART2_BASE)
        CLEAR_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
#endif // USART2_BASE

#ifdef USART3_BASE
    if constexpr (Base == USART3_BASE)
        CLEAR_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);
#endif // USART3_BASE

#ifdef UART4_BASE
    if constexpr (Base == UART4_BASE)
        CLEAR_BIT(RCC->APB1ENR, RCC_APB1ENR_UART4EN);
#endif // USRT4_BASE

#ifdef UART5_BASE
    if constexpr (Base == UART5_BASE)
        CLEAR_BIT(RCC->APB1ENR, RCC_APB1ENR_UART5EN);
#endif // UART5_BASE
}

/**
 * @brief 对本外设进行默认配置
 * 
 * @tparam Base 
 * @param baud 波特率(默认115200)
 */
template <uint32_t Base>
void Serial<Base>::defult(uint32_t baud)
{
    static_assert(is_valid());
    baudrate(baud);
    stopbits_1();
    wordlength_9bits();
    parity_none();
    transmitter(true);
    receiver(true);
}

/**
 * @brief 设置波特率
 * 
 * @tparam Base 
 * @param val 波特率
 */
template <uint32_t Base>
void Serial<Base>::baudrate(uint32_t val)
{
    uint32_t fck;
    double div;
    uint32_t mantissa;
    uint32_t fraction;

    static_assert(is_valid());

    Rcc::update_clocks();

    fck = (Base != USART1_BASE) ? Rcc::get_clocks().pclk1 : Rcc::get_clocks().pclk2;
    div = fck / (16.0 * val);
    mantissa = (uint32_t)div;
    fraction = (uint32_t)(16 * (div - mantissa) + 0.5);

    WRITE_REG(instance->BRR, (mantissa << USART_BRR_DIV_Mantissa_Pos) |
                                 (fraction << USART_BRR_DIV_Fraction_Pos));
}

/**
 * @brief 停止位为1bit
 * 
 * @tparam Base 
 */
template <uint32_t Base>
inline void Serial<Base>::stopbits_1(void)
{
    static_assert(is_valid());
    CLEAR_BIT(instance->CR2, USART_CR2_STOP);
}

/**
 * @brief 停止位为0.5bit
 * 
 * @tparam Base 
 */
template <uint32_t Base>
inline void Serial<Base>::stopbits_0P5(void)
{
    static_assert(is_valid());
    MODIFY_REG(instance->CR2, USART_CR2_STOP, 1 << USART_CR2_STOP_Pos);
}

/**
 * @brief 停止位为2bit
 * 
 * @tparam Base 
 */
template <uint32_t Base>
inline void Serial<Base>::stopbits_2(void)
{
    static_assert(is_valid());
    MODIFY_REG(instance->CR2, USART_CR2_STOP, 2 << USART_CR2_STOP_Pos);
}

/**
 * @brief 停止位为1.5bit
 * 
 * @tparam Base 
 */
template <uint32_t Base>
inline void Serial<Base>::stopbits_1P5(void)
{
    static_assert(is_valid());
    SET_BIT(instance->CR2, USART_CR2_STOP);
}

/**
 * @brief 字长为8bit
 * 
 * @tparam Base 
 */
template <uint32_t Base>
inline void Serial<Base>::wordlength_8bits(void)
{
    static_assert(is_valid());
    CLEAR_BIT(instance->CR2, USART_CR1_M);
}

/**
 * @brief 字长为9bit
 * 
 * @tparam Base 
 */
template <uint32_t Base>
inline void Serial<Base>::wordlength_9bits(void)
{
    static_assert(is_valid());
    SET_BIT(instance->CR2, USART_CR1_M);
}

/**
 * @brief 无奇偶校验
 * 
 * @tparam Base 
 */
template <uint32_t Base>
inline void Serial<Base>::parity_none(void)
{
    static_assert(is_valid());
    CLEAR_BIT(instance->CR2, USART_CR1_PCE | USART_CR1_RE);
}

/**
 * @brief 偶校验
 * 
 * @tparam Base 
 */
template <uint32_t Base>
inline void Serial<Base>::parity_even(void)
{
    static_assert(is_valid());
    MODIFY_REG(instance->CR2, USART_CR1_PCE | USART_CR1_RE, USART_CR1_PCE);
}

/**
 * @brief 奇校验
 * 
 * @tparam Base 
 */
template <uint32_t Base>
inline void Serial<Base>::parity_odd(void)
{
    static_assert(is_valid());
    SET_BIT(instance->CR2, USART_CR1_PCE | USART_CR1_RE);
}

/**
 * @brief 外设使能
 * 
 * @tparam Base 
 */
template <uint32_t Base>
void Serial<Base>::enable(void)
{
    static_assert(is_valid());
    SET_BIT(instance->CR1, USART_CR1_UE);
}

/**
 * @brief 外设禁止
 * 
 * @tparam Base 
 */
template <uint32_t Base>
void Serial<Base>::disable(void)
{
    static_assert(is_valid());
    CLEAR_BIT(instance->CR1, USART_CR1_UE);
}

/**
 * @brief 发送使能/禁止
 * 
 * @tparam Base 
 * @param enable 
 * @arg true 使能
 * @arg false 禁止
 */
template <uint32_t Base>
void Serial<Base>::transmitter(bool enable)
{
    static_assert(is_valid());
    MODIFY_REG(instance->CR1, USART_CR1_TE, enable ? USART_CR1_TE : 0);
}

/**
 * @brief 接受使能/禁止
 * 
 * @tparam Base 
 * @param enable 
 * @arg true 使能
 * @arg false 禁止
 */
template <uint32_t Base>
void Serial<Base>::receiver(bool enable)
{
    static_assert(is_valid());
    MODIFY_REG(instance->CR1, USART_CR1_RE, enable ? USART_CR1_RE : 0);
}

/**
 * @brief DMA发送使能/禁止
 * 
 * @tparam Base 
 * @param enable 
 * @arg true 使能
 * @arg false 禁止
 */
template <uint32_t Base>
void Serial<Base>::request_transmitter_from_dma(bool enable)
{
    static_assert(is_valid());
    MODIFY_REG(instance->CR3, USART_CR3_DMAT, enable ? USART_CR3_DMAT : 0);
}

/**
 * @brief DMA接收使能/禁止
 * 
 * @tparam Base 
 * @param enable 
 * @arg true 使能
 * @arg false 禁止
 */
template <uint32_t Base>
void Serial<Base>::request_receiver_from_dma(bool enable)
{
    static_assert(is_valid());
    MODIFY_REG(instance->CR3, USART_CR3_DMAR, enable ? USART_CR3_DMAR : 0);
}

/**
 * @brief 向数据寄存器写一个数据
 * 
 * @tparam Base 
 * @param val 数据
 */
template <uint32_t Base>
void Serial<Base>::write_dr(uint32_t val)
{
    static_assert(is_valid());
    WRITE_REG(instance->DR, val);
}


/**
 * @brief 从数据寄存器读一个数据
 * 
 * @tparam Base 
 */
template <uint32_t Base>
uint32_t Serial<Base>::read_dr(void)
{
    static_assert(is_valid());
    return READ_REG(instance->DR);
}

/**
 * @brief 发送一个字符
 * 
 * @tparam Base 
 * @param character 字符
 */
template <uint32_t Base>
void Serial<Base>::putchar(char character)
{
    static_assert(is_valid());
    while (!flag.txe.is_set());
    write_dr(character);
    while (!flag.tc.is_set());
    flag.tc.clear();
}

/**
 * @brief 发送一个字符
 * 
 * @tparam Base 
 * @param character 字符
 */
template <uint32_t Base>
char Serial<Base>::getchar(void)
{
    static_assert(is_valid());
    while (!flag.rxne.is_set());
    return read_dr();
}

/**
 * @brief 发送一组数据
 * 
 * @tparam Base 
 * @param ptr 数据首地址
 * @param size 数据量
 */
template <uint32_t Base>
void Serial<Base>::transmit(void *ptr, size_t size)
{
    static_assert(is_valid());

    if (ptr == nullptr)
    {
        return;
    }

    for (size_t i = 0; i < size; i++)
    {
        putchar(((char *)ptr)[i]);
    }
}

/**
 * @brief 接收一组数据
 * 
 * @tparam Base 
 * @param ptr 待存放数据首地址
 * @param size 数据量
 */
template <uint32_t Base>
void Serial<Base>::receive(void *ptr, size_t size)
{
    static_assert(is_valid());

    if (ptr == nullptr)
    {
        return;
    }

    for (size_t i = 0; i < size; i++)
    {
        ((char *)ptr)[i] = getchar();
    }
}

/**
 * @brief 格式化发送字符串
 * 
 * @tparam Base 
 * @tparam Args 
 * @param format 格式化字符串
 * @param args 占位符对应的参数
 */
template <uint32_t Base>
template <typename... Args>
void Serial<Base>::print(const char *format, const Args &...args)
{
    static_assert(is_valid());
    fmt::putchar = putchar;
    fmt::print(format, args...);
}

/**
 * @brief 格式化发送字符串并换行
 * 
 * @tparam Base 
 * @tparam Args 
 * @param format 格式化字符串
 * @param args 占位符对应的参数
 */
template <uint32_t Base>
template <typename... Args>
void Serial<Base>::println(const char *format, const Args &...args)
{
    static_assert(is_valid());
    fmt::putchar = putchar;
    fmt::println(format, args...);
}

/**
 * @brief 判断模板参数合法性
 * 
 * @tparam Base 
 * @return true 合法
 * @return false 非法
 */
template <uint32_t Base>
inline constexpr bool Serial<Base>::is_valid(void)
{
#ifdef USART1_BASE
    if constexpr (Base == USART1_BASE)
        return true;
#endif // USART1_BASE

#ifdef USART2_BASE
    if constexpr (Base == USART2_BASE)
        return true;
#endif // USART2_BASE

#ifdef USART3_BASE
    if constexpr (Base == USART3_BASE)
        return true;
#endif // USART3_BASE

#ifdef UART4_BASE
    if constexpr (Base == UART4_BASE)
        return true;
#endif // USRT4_BASE

#ifdef UART5_BASE
    if constexpr (Base == UART5_BASE)
        return true;
#endif // UART5_BASE
}

#endif // !__SERIAL_HPP__
