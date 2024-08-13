#ifndef __FORMATTER_STRING_HPP__
#define __FORMATTER_STRING_HPP__

#include "utils.hpp"
#include "WString.h"

namespace fmt
{
    struct StrFormaterOption
    {
        char fill;
        Align align;
        uint8_t width;
    };
} // namespace fmt

template <>
struct fmt::Formatter<char *>
{
    static String format(Context &context, const char *arg);
    static size_t print(Context &context, const char *arg);
};

template <size_t size>
struct fmt::Formatter<char [size]>
{
    static String format(Context &context, const char (&arg)[size]);
    static size_t print(Context &context, const char (&arg)[size]);
};

template <>
struct fmt::Formatter<String>
{
    static String format(Context &context, const String &arg);
    static size_t print(Context &context, const String &arg);
};

template <size_t size>
size_t fmt::Formatter<char [size]>::print(fmt::Context &context, const char (&arg)[size])
{
    int fill_width;
    size_t length;
    StrFormaterOption option{};

    init(context, option);

    length = strlen(arg);
    fill_width = (option.width > length) ? (option.width - length) : 0;

    switch (option.align)
    {
    case Align::Left:
    {
        write(arg, length);

        for (int i = 0; i < fill_width; i++)
        {
            write(&option.fill, 1);
        }

        break;
    }

    case Align::Centering:
    {
        for (int i = 0; i < fill_width / 2; i++)
        {
            write(&option.fill, 1);
        }

        write(arg, length);

        for (int i = 0; i < (fill_width + 1) / 2; i++)
        {
            write(&option.fill, 1);
        }

        break;
    }
    case Align::Right:
    {
        for (int i = 0; i < fill_width; i++)
        {
            write(&option.fill, 1);
        }

        write(arg, length);

        break;
    }
    
    default:
        break;
    }

    return 1;
}

#endif // !__FORMATTER_STRING_HPP__
