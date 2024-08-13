#ifndef __FORMATER_INT_HPP__
#define __FORMATER_INT_HPP__

#include <type_traits>
#include "utils.hpp"
#include "WString.h"
#include "itoa.h"

namespace fmt
{
    enum IntType : char
    {
        Bin = 'b',
        Oct = 'o',
        Dec = 'd',
        hex = 'x',
        Hex = 'X',
        Bol = 's',
        Chr = 'c',
    };

    static inline bool is_int_type(char ch)
    {
        return (ch == 'b') || (ch == 'o') || (ch == 'd') || (ch == 'x') || (ch == 'X') || (ch == 's') || (ch == 'c');
    }

    struct IntFormaterOption
    {
        char fill;
        Align align;
        Sign sign;
        uint8_t width;
        IntType type;
    };
} // namespace fmt

template <>
struct fmt::Formatter<bool>
{
    static String format(Context &context, bool arg);
    static size_t print(Context &context, bool arg);
};

template <>
struct fmt::Formatter<char>
{
    static String format(Context &context, char arg);
    static size_t print(Context &context, char arg);
};

template <>
struct fmt::Formatter<signed char>
{
    static String format(Context &context, signed char arg);
    static size_t print(Context &context, signed char arg);
};

template <>
struct fmt::Formatter<unsigned char>
{
    static String format(Context &context, unsigned char arg);
    static size_t print(Context &context, unsigned char arg);
};

template <>
struct fmt::Formatter<signed short>
{
    static String format(Context &context, signed short arg);
    static size_t print(Context &context, signed short arg);
};

template <>
struct fmt::Formatter<unsigned short>
{
    static String format(Context &context, unsigned short arg);
    static size_t print(Context &context, unsigned short arg);
};

template <>
struct fmt::Formatter<signed int>
{
    static String format(Context &context, signed int arg);
    static size_t print(Context &context, signed int arg);
};

template <>
struct fmt::Formatter<unsigned int>
{
    static String format(Context &context, unsigned int arg);
    static size_t print(Context &context, unsigned int arg);
};

template <>
struct fmt::Formatter<signed long>
{
    static String format(Context &context, signed long arg);
    static size_t print(Context &context, signed long arg);
};

template <>
struct fmt::Formatter<unsigned long>
{
    static String format(Context &context, unsigned long arg);
    static size_t print(Context &context, unsigned long arg);
};

template <>
struct fmt::Formatter<signed long long>
{
    static String format(Context &context, signed long long arg);
    static size_t print(Context &context, signed long long arg);
};

template <>
struct fmt::Formatter<unsigned long long>
{
    static String format(Context &context, unsigned long long arg);
    static size_t print(Context &context, unsigned long long arg);
};

#endif // !__FORMATER_INT_HPP__
