#ifndef __CRC_HPP__
#define __CRC_HPP__

#include "stm32f1xx.h"
#include "bitband.h"
#include "stddef.h"

class Crc
{
private:

public:
    static void clock(bool enable) { MODIFY_REG(RCC->AHBENR, RCC_AHBENR_CRCEN, enable ? RCC_AHBENR_CRCEN : 0); }
    static void reset(void) { WRITE_REG(CRC->CR, CRC_CR_RESET); }
    static void write(uint32_t data) { WRITE_REG(CRC->DR, data); }
    static uint32_t read(void) { return READ_REG(CRC->DR); }
    static void store(uint8_t data) { WRITE_REG(CRC->IDR, data); }

    static uint32_t calculate(const uint32_t *ptr, size_t length)
    {
        reset();

        for (const uint32_t *iter = ptr; (size_t)(iter - ptr) < length; iter++)
        {
            write(*iter);
        }

        return read();
    }

    static bool check(const uint32_t *ptr, size_t length)
    {
        reset();

        for (const uint32_t *iter = ptr; (size_t)(iter - ptr) < length; iter++)
        {
            write(*iter);
        }

        return read() == 0;
    }
};

inline Crc crc;

#endif // !__CRC_HPP__
