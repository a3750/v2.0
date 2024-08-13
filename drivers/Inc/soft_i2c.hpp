#ifndef __SOFT_I2C_HPP__
#define __SOFT_I2C_HPP__

#include "gpio.hpp"

template <class scl, class sda>
class SoftI2C
{
private:

public:
    static void init(void);
    static void start(void);
    static void stop(void);
    static void writebyte(uint8_t byte);
    static uint8_t readbyte(void);
    static void ack(void);
    static void no_ack(void);
    static bool wait_ack(uint32_t delay = 10);
};

template <typename T>
struct is_specialization_of_soft_i2c
{
    static constexpr bool value = false;
};

template <typename scl, typename sda>
struct is_specialization_of_soft_i2c<SoftI2C<scl, sda>>
{
    static constexpr bool value = is_specialization_of_gpio<scl>::value && is_specialization_of_gpio<sda>::value;
};

template <class scl, class sda>
inline void SoftI2C<scl, sda>::init(void)
{
    static_assert(is_specialization_of_gpio<scl>::value);
    static_assert(is_specialization_of_gpio<sda>::value);

    scl::enable_clock();
    sda::enable_clock();

    scl::odr = 1;
    sda::odr = 1;

    scl::into_output().push_pull();
    sda::into_output().push_pull();
}

template <class scl, class sda>
inline void SoftI2C<scl, sda>::start(void)
{
    static_assert(is_specialization_of_gpio<scl>::value);
    static_assert(is_specialization_of_gpio<sda>::value);

    sda::odr = 1;
    scl::odr = 1;
    sda::odr = 0;
    scl::odr = 0;
}

template <class scl, class sda>
inline void SoftI2C<scl, sda>::stop(void)
{
    static_assert(is_specialization_of_gpio<scl>::value);
    static_assert(is_specialization_of_gpio<sda>::value);

    sda::odr = 0;
    scl::odr = 1;
    sda::odr = 1;
}

template <class scl, class sda>
void SoftI2C<scl, sda>::writebyte(uint8_t byte)
{
    static_assert(is_specialization_of_gpio<scl>::value);
    static_assert(is_specialization_of_gpio<sda>::value);

    for (size_t i = 0; i < 8; i++, byte <<= 1)
    {
        sda::odr = byte >> 7;
        scl::odr = 1;
        scl::odr = 0;
    }
    sda::odr = 1;
}

template <class scl, class sda>
uint8_t SoftI2C<scl, sda>::readbyte(void)
{
    static_assert(is_specialization_of_gpio<scl>::value);
    static_assert(is_specialization_of_gpio<sda>::value);

    uint8_t byte = 0;
    sda::into_input().pull_up();
    for (size_t i = 0; i < 8; i++)
    {
        scl::odr = 1;
        byte = (byte << 1) | sda::idr;
        scl::odr = 0;
    }
    sda::into_output().push_pull();
    return byte;
}

template <class scl, class sda>
inline void SoftI2C<scl, sda>::ack(void)
{
    static_assert(is_specialization_of_gpio<scl>::value);
    static_assert(is_specialization_of_gpio<sda>::value);

    sda::odr = 0;
    scl::odr = 1;
    scl::odr = 0;
}

template <class scl, class sda>
inline void SoftI2C<scl, sda>::no_ack(void)
{
    static_assert(is_specialization_of_gpio<scl>::value);
    static_assert(is_specialization_of_gpio<sda>::value);

    sda::odr = 1;
    scl::odr = 1;
    scl::odr = 0;
}

template <class scl, class sda>
bool SoftI2C<scl, sda>::wait_ack(uint32_t delay)
{
    static_assert(is_specialization_of_gpio<scl>::value);
    static_assert(is_specialization_of_gpio<sda>::value);

    bool ack = false;
    sda::into_input().pull_up();
    scl::odr = 1;
    for (uint32_t i = 0; i < delay; i++)
    {
        if (sda::idr == 0)
        {
            ack = true;
            break;
        }
    }
    scl::odr = 0;
    sda::into_input().push_pull();
    return ack;
}

#endif // !__SOFT_I2C_HPP__
