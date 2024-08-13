#ifndef __FMT_HPP__
#define __FMT_HPP__

#include "formatter/int.hpp"
#include "formatter/float.hpp"
#include "formatter/str.hpp"

/**
 * @brief // [fill][align][sign][width].[precision][type]
 * @param fill 空白处填充的字符
 * @param align '<' '^' '>'
 * @param sign '+' '-' ' '
 * @param width 宽度
 * @param precision 精度
 * @param type 类型
 */

namespace fmt
{
    template <typename... Args>
    String format(const char *fmt, const Args &...args);

    template <typename... Args>
    size_t print(const char *fmt, const Args &...args);

    template <typename... Args>
    size_t println(const char *fmt, const Args &...args);

    template <size_t Index, typename Arg, typename... Args>
    String format_helper(size_t index, Context &context, const Arg &arg, const Args &...args);

    template <size_t Index, typename Arg, typename... Args>
    size_t print_helper(size_t index, Context &context, const Arg &arg, const Args &...args);
} // namespace fmt

template <typename... Args>
String fmt::format(const char *fmt, const Args &...args)
{
    String result;
    size_t index = 0;
    Context context;

    if (fmt == nullptr)
    {
        return result;
    }

    while (*fmt != '\0')
    {
        switch (*fmt)
        {
        case '{':
        {
            const char *temp = fmt + 1;
            context.begin = fmt + 1;
            do
            {
                switch (*(++fmt))
                {                
                case ':':
                    context.begin = fmt + 1;
                    break;

                case '}':
                    context.end = fmt;
                    break;

                default:
                    break;
                }
            } while (*fmt != '{' && *fmt != '}' && *fmt != '\0');

            if (*fmt == '{')
            {
                result += *fmt++;
            }
            else if (*fmt == '}')
            {
                if (*temp >= '0' && *temp <= '9')
                {
                    result += format_helper<0>(atoi(temp), context, args...);
                }
                else
                {
                    result += format_helper<0>(index++, context, args...);
                }
                fmt++;
            }

            break;
        }

        case '}':
        {
            do { fmt++; } while (*fmt != '}' && *fmt != '\0');
            if (*fmt == '}')
            {
                result += *fmt++;
            }
            break;
        }

        default:
            result += *fmt++;
            break;
        }
    }

    return result;
}

template <typename... Args>
size_t fmt::print(const char *fmt, const Args &...args)
{
    size_t count = 0, index = 0;
    Context context;

    if (fmt == nullptr)
    {
        return count;
    }

    while (*fmt != '\0')
    {
        switch (*fmt)
        {
        case '{':
        {
            const char *temp = fmt + 1;
            context.begin = fmt + 1;
            do
            {
                switch (*(++fmt))
                {                
                case ':':
                    context.begin = fmt + 1;
                    break;

                case '}':
                    context.end = fmt;
                    break;

                default:
                    break;
                }
            } while (*fmt != '{' && *fmt != '}' && *fmt != '\0');

            if (*fmt == '{')
            {
                write(fmt++, 1);
            }
            else if (*fmt == '}')
            {
                if (*temp >= '0' && *temp <= '9')
                {
                    count += print_helper<0>(atoi(temp), context, args...);
                }
                else
                {
                    print_helper<0>(index++, context, args...);
                }
                fmt++;
            }

            break;
        }

        case '}':
        {
            do { fmt++; } while (*fmt != '}' && *fmt != '\0');
            if (*fmt == '}')
            {
                write(fmt++, 1);
            }
            break;
        }

        default:
            write(fmt++, 1);
            break;
        }
    }

    return count;
}

template <typename... Args>
size_t fmt::println(const char *fmt, const Args &...args)
{
    size_t ret = print(fmt, args...);
    write("\n", 1);
    return ret;
}

template <size_t Index, typename Arg, typename... Args>
String fmt::format_helper(size_t index, Context &context, const Arg &arg, const Args &...args)
{
    if (index == Index)
    {
        return (Formatter<Arg>::format(context, arg));
    }

    if constexpr (sizeof...(Args) != 0)
    {
        return (format_helper<Index + 1>(index, context, args...));
    }
    else
    {
        return (String(""));
    }
}

template <size_t Index, typename Arg, typename... Args>
size_t fmt::print_helper(size_t index, Context &context, const Arg &arg, const Args &...args)
{
    if (index == Index)
    {
        return Formatter<Arg>::print(context, arg);
    }

    if constexpr (sizeof...(Args) != 0)
    {
        return print_helper<Index + 1>(index, context, args...);
    }
    else
    {
        return 0;
    }
}

#endif // !__FMT_HPP__
