#ifndef __FORMAT_FLOAT__
#define __FORMAT_FLOAT__

#include <cstdio>
#include <type_traits>
#include "utils.hpp"
#include "WString.h"

#define __USING_FLOAT_MAX__ 1e16
#define __USING_FLOAT_MIN__ 1e-4

namespace fmt
{
    enum FloatType : char
    {
        Float = 'f',
        Exp = 'e',
    };

    static inline bool is_float_type(char ch)
    {
        return (ch == 'f') || (ch == 'e');
    }

    struct FloatFormaterOption
    {
        char fill;
        Align align;
        Sign sign;
        uint8_t width;
        bool auto_precision;
        uint8_t precision;
        FloatType type;
    };

    template <typename T>
    void ftoa(T value, char *buffer, uint8_t precision);

    template <typename T>
    void ftoa(T value, char *buffer);
} // namespace fmt

template <>
struct fmt::Formatter<float>
{
    static String format(Context &context, float arg);
    static size_t print(Context &context, float arg);
};

template <>
struct fmt::Formatter<double>
{
    static String format(Context &context, double arg);
    static size_t print(Context &context, double arg);
};

#endif // !__FORMAT_FLOAT__
