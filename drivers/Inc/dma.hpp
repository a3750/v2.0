#ifndef __DMA_HPP__
#define __DMA_HPP__

#include "bitband.h"
#include "stm32f1xx.h"

template <uint32_t Base, uint8_t Channel>
struct DmaChannel
{
    static DMA_TypeDef *instance;
    static DMA_Channel_TypeDef *channel;

    static constexpr uint32_t clock_msk = (Base == DMA1_BASE) ? RCC_AHBENR_DMA1EN : RCC_AHBENR_DMA2EN;

    static constexpr bool is_valid(void) { return ((Base == DMA1_BASE) || (Base == DMA2_BASE)) && (Channel < 7); }
    static void enable_clock(void);
    static void disable_clock(void);
    static void clock(bool enable);
    static void memory_to_memory(bool enable);
    static void priority_low(void);
    static void priority_medium(void);
    static void priority_high(void);
    static void priority_highest(void);
    static void memory_size_8bits(void);
    static void memory_size_16bits(void);
    static void memory_size_32bits(void);
    static void periph_size_8bits(void);
    static void periph_size_16bits(void);
    static void periph_size_32bits(void);
    static void memory_increment(bool enable);
    static void periph_increment(bool enable);
    static void circular(bool enable);
    static void data_from_memory(void);
    static void data_from_periph(void);
    static void enable(void);
    static void disable(void);
    static void set_length(uint16_t length);
    static uint32_t get_length(void);
    static void memory_address(volatile void *address);
    static void periph_address(volatile void *address);
};

template <typename T>
struct is_specialization_of_dma
{
    static constexpr bool value = false;
};

template <uint32_t Base, uint8_t Channel>
struct is_specialization_of_dma<DmaChannel<Base, Channel>>
{
    static constexpr bool value = DmaChannel<Base, Channel>::is_valid();
};

template <uint8_t Channel>
using Dma1 = DmaChannel<DMA1_BASE, Channel>;

template <uint8_t Channel>
using Dma2 = DmaChannel<DMA2_BASE, Channel>;

template <uint8_t Channel>
Dma1<Channel> dma1;

template <uint8_t Channel>
Dma2<Channel> dma2;

/**
 * @brief DMA外设结构体指针
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
DMA_TypeDef *DmaChannel<Base, Channel>::instance = (DMA_TypeDef *)Base;

/**
 * @brief DMA外设通道结构体指针
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
DMA_Channel_TypeDef *DmaChannel<Base, Channel>::channel = (DMA_Channel_TypeDef *)(Base + sizeof(DMA_TypeDef) + Channel * (sizeof(DMA_Channel_TypeDef) + sizeof(uint32_t)));

/**
 * @brief 时钟使能
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::enable_clock(void)
{
    static_assert(is_valid());
    SET_BIT(RCC->AHBENR, clock_msk);
}

/**
 * @brief 时钟禁止
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::disable_clock(void)
{
    static_assert(is_valid());
    CLEAR_BIT(RCC->AHBENR, clock_msk);
}

/**
 * @brief 时钟使能/禁止
 * 
 * @tparam Base 
 * @tparam Channel 
 * @param enable 
 * @arg true 使能
 * @arg false 禁止
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::clock(bool enable)
{
    static_assert(is_valid());
    MODIFY_REG(RCC->AHBENR, clock_msk, enable ? clock_msk : 0);
}

/**
 * @brief 存储器到存储器模式使能/禁止
 * 
 * @tparam Base 
 * @tparam Channel 
 * @param enable 
 * @arg true 使能
 * @arg false 禁止
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::memory_to_memory(bool enable)
{
    static_assert(is_valid());
    MODIFY_REG(channel->CCR, DMA_CCR_MEM2MEM, enable ? DMA_CCR_MEM2MEM : 0);
}

/**
 * @brief 优先级设置为低
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::priority_low(void)
{
    static_assert(is_valid());
    CLEAR_BIT(channel->CCR, DMA_CCR_PL);
}

/**
 * @brief 优先级设置为中
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::priority_medium(void)
{
    static_assert(is_valid());
    MODIFY_REG(channel->CCR, DMA_CCR_PL, 1 << DMA_CCR_PL_Pos);
}

/**
 * @brief 优先级设置为高
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::priority_high(void)
{
    static_assert(is_valid());
    MODIFY_REG(channel->CCR, DMA_CCR_PL, 2 << DMA_CCR_PL_Pos);
}

/**
 * @brief 优先级设置为最高
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::priority_highest(void)
{
    static_assert(is_valid());
    SET_BIT(channel->CCR, DMA_CCR_PL);
}

/**
 * @brief 存储器数据宽度设置为8bit
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::memory_size_8bits(void)
{
    static_assert(is_valid());
    CLEAR_BIT(channel->CCR, DMA_CCR_MSIZE);
}

/**
 * @brief 存储器数据宽度设置为16bit
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::memory_size_16bits(void)
{
    static_assert(is_valid());
    MODIFY_REG(channel->CCR, DMA_CCR_MSIZE, 1 << DMA_CCR_MSIZE_Pos);
}

/**
 * @brief 存储器数据宽度设置为32bit
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::memory_size_32bits(void)
{
    static_assert(is_valid());
    MODIFY_REG(channel->CCR, DMA_CCR_MSIZE, 2 << DMA_CCR_MSIZE_Pos);
}

/**
 * @brief 外设数据宽度设置为8bit
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::periph_size_8bits(void)
{
    static_assert(is_valid());
    CLEAR_BIT(channel->CCR, DMA_CCR_PSIZE);
}

/**
 * @brief 外设数据宽度设置为16bit
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::periph_size_16bits(void)
{
    static_assert(is_valid());
    MODIFY_REG(channel->CCR, DMA_CCR_PSIZE, 1 << DMA_CCR_PSIZE_Pos);
}

/**
 * @brief 外设数据宽度设置为32bit
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::periph_size_32bits(void)
{
    static_assert(is_valid());
    MODIFY_REG(channel->CCR, DMA_CCR_PSIZE, 2 << DMA_CCR_PSIZE_Pos);
}

/**
 * @brief 存储器地址增量模式使能/禁止
 * 
 * @tparam Base 
 * @tparam Channel 
 * @param enable 
 * @arg true 使能
 * @arg false 禁止
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::memory_increment(bool enable)
{
    static_assert(is_valid());
    MODIFY_REG(channel->CCR, DMA_CCR_MINC, enable ? DMA_CCR_MINC : 0);
}

/**
 * @brief 外设地址增量模式使能/禁止
 * 
 * @tparam Base 
 * @tparam Channel 
 * @param enable 
 * @arg true 使能
 * @arg false 禁止
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::periph_increment(bool enable)
{
    static_assert(is_valid());
    MODIFY_REG(channel->CCR, DMA_CCR_PINC, enable ? DMA_CCR_PINC : 0);
}

/**
 * @brief 循环模式使能/禁止
 * 
 * @tparam Base 
 * @tparam Channel 
 * @param enable 
 * @arg true 使能
 * @arg false 禁止
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::circular(bool enable)
{
    static_assert(is_valid());
    MODIFY_REG(channel->CCR, DMA_CCR_CIRC, enable ? DMA_CCR_CIRC : 0);
}

/**
 * @brief 数据传输方向设置为存储器->外设
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::data_from_memory(void)
{
    static_assert(is_valid());
    SET_BIT(channel->CCR, DMA_CCR_DIR);
}

/**
 * @brief 数据传输方向设置为外设->存储器
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::data_from_periph(void)
{
    static_assert(is_valid());
    CLEAR_BIT(channel->CCR, DMA_CCR_DIR);
}

/**
 * @brief DMA外设使能
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::enable(void)
{
    static_assert(is_valid());
    SET_BIT(channel->CCR, DMA_CCR_EN);
}

/**
 * @brief DMA外设禁止
 * 
 * @tparam Base 
 * @tparam Channel 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::disable(void)
{
    static_assert(is_valid());
    CLEAR_BIT(channel->CCR, DMA_CCR_EN);
}

/**
 * @brief 设置传输数据的长度(DMA外设启动时无效)
 * 
 * @tparam Base 
 * @tparam Channel 
 * @param length 
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::set_length(uint16_t length)
{
    static_assert(is_valid());
    WRITE_REG(channel->CNDTR, length);
}

/**
 * @brief 获取剩余待传输数据的长度
 * 
 * @tparam Base 
 * @tparam Channel 
 * @return uint32_t 
 */
template <uint32_t Base, uint8_t Channel>
inline uint32_t DmaChannel<Base, Channel>::get_length(void)
{
    static_assert(is_valid());
    return READ_REG(channel->CNDTR);
}

/**
 * @brief 设置存储器(首)地址
 * 
 * @tparam Base 
 * @tparam Channel 
 * @param address 地址
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::memory_address(volatile void *address)
{
    static_assert(is_valid());
    WRITE_REG(channel->CMAR, (uint32_t)address);
}

/**
 * @brief 设置外设(首)地址
 * 
 * @tparam Base 
 * @tparam Channel 
 * @param address 地址
 */
template <uint32_t Base, uint8_t Channel>
inline void DmaChannel<Base, Channel>::periph_address(volatile void *address)
{
    static_assert(is_valid());
    WRITE_REG(channel->CPAR, (uint32_t)address);
}

#endif // !__DMA_HPP__
