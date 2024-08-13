#ifndef __FORMATER_HPP__
#define __FORMATER_HPP__

#include "stdint.h"
#include "stddef.h"

namespace fmt
{
    struct Context
    {
        const char *begin;
        const char *end;
    };

    enum Align : char
    {
        Left = '<',
        Centering = '^',
        Right = '>',
    };

    enum Sign : char
    {
        Plus = '+',
        Minus = '-',
        Space = ' ',
    };

    static inline bool is_align(char character)
    {
        return (character == '<') || (character == '^') || (character == '>');
    }

    static inline bool is_sign(char character)
    {
        return (character == '+') || (character == '-') || (character == ' ');
    }

    static inline bool is_digit(char character)
    {
        return (character >= '0') && (character <= '9');
    }

    template <typename T>
    static inline T abs(T n)
    {
        return (n > 0) ? n : -n;
    }

    template <typename T>
    struct Formatter {};

    template <typename T>
    void init(Context &context, T &option) {}

    inline void (*putchar)(char character);

    inline void write(const char *ptr, int len)
    {
        if (putchar == nullptr)
        {
            return;
        }

        for (int i = 0; i < len; i++)
        {
            putchar(ptr[i]);
        }
    }
} // namespace fmt


#endif // !__FORMATER_HPP__
