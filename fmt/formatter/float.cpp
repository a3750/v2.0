#include "float.hpp"
#include "itoa.h"

template <>
void fmt::init(Context &context, FloatFormaterOption &option)
{
    auto iter = context.begin;
    option = {' ', Align::Right, Sign::Minus, 0, true, 0, option.type};

    if (iter >= context.end || iter == nullptr)
    {
        return;
    }

    if (iter + 1 < context.end && is_align(*(iter + 1)))
    {
        option.fill = *iter;
        option.align = static_cast<Align>(*(iter + 1));
        iter += 2;
    }
    else if (is_align(*iter))
    {
        option.align = static_cast<Align>(*iter);
        iter++;
    }

    if (iter >= context.end)
    {
        return;
    }

    if (is_sign(*iter))
    {
        option.sign = static_cast<Sign>(*iter);
        iter++;
    }

    if (iter >= context.end)
    {
        return;
    }

    if (is_digit(*iter))
    {
        do
        {
            option.width = option.width * 10 + *iter++ - '0';
        } while (is_digit(*iter) && iter < context.end);
    }

    if (iter >= context.end)
    {
        return;
    }

    if (*iter == '.')
    {
        option.auto_precision = false;
        iter++;
        while (is_digit(*iter) && iter < context.end)
        {
            option.precision = option.precision * 10 + *iter++ - '0';
        }
    }

    if (iter >= context.end)
    {
        return;
    }

    if (is_float_type(*iter))
    {
        option.type = static_cast<FloatType>(*iter);
    }
}

template <typename T>
void fmt::ftoa(T value, char *buffer)
{
    if (value < 0)
    {
        buffer[0] = '-';
        value = -value;
        buffer++;
    }

    itoa(value, buffer, 10);
    buffer += strlen(buffer);
    *buffer++ = '.';
    value -= (int)value;

    for (char ch; value >= 1e-4;)
    {
        value *= 10;
        ch = (char)value;
        value -= ch;
        ch += '0';
        if (1 - value < 1e-4)
        {
            ch++;
            value = 0;
        }
        *buffer++ = ch;
    }

    *buffer = '\0';
}

template <typename T>
void fmt::ftoa(T value, char *buffer, uint8_t precision)
{
    char ch;

    if (value < 0)
    {
        buffer[0] = '-';
        value = -value;
        buffer++;
    }

    itoa(value, buffer, 10);
    buffer += strlen(buffer);
    *buffer++ = '.';
    value -= (int)value;

    for (uint8_t i = 0; i < precision - 1; i++)
    {
        value *= 10;
        ch = (char)value;
        value -= ch;
        ch += '0';
        *buffer++ = ch;
    }
    ch = (char)(value * 10 + 0.5) + '0';
    *buffer++ = ch;
    *buffer = '\0';
}

size_t fmt::Formatter<float>::print(Context &context, float arg)
{
    char buffer[64];
    uint8_t offset;
    size_t length;
    int fill_width;
    FloatFormaterOption option;

    // Check if the argument is within the range for float type
    if (arg >= __USING_FLOAT_MIN__ && arg < __USING_FLOAT_MAX__)
    {
        option.type = FloatType::Float;
    }
    else
    {
        option.type = FloatType::Exp;
    }

    init(context, option);

    if (arg < 0)
    {
        arg = -arg;
        buffer[0] = '-';
        offset = 1;
    }
    else if (option.sign != Sign::Minus)
    {
        buffer[0] = static_cast<char>(option.sign);
        offset = 1;
    }
    else
    {
        offset = 0;
    }

    switch (option.type)
    {
    case FloatType::Exp:
    {
        char format_string[7];
        option.precision = (option.auto_precision) ? 6 : option.precision;
        snprintf(format_string, sizeof(format_string), "%%.%de", option.precision);
        snprintf(buffer + offset, sizeof(buffer) - offset, format_string, arg);
        break;
    }

    case FloatType::Float:
    {
        if (option.auto_precision)
        {
            ftoa(arg, buffer + offset);
        }
        else
        {
            ftoa(arg, buffer + offset, option.precision);
        }
        break;
    }

    default:
        break;
    }

    length = strlen(buffer);
    fill_width = (length < option.width) ? (option.width - length) : 0;

    switch (option.align)
    {
    case Align::Left:
    {
        write(buffer, length);

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

        write(buffer, length);

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

        write(buffer, length);

        break;
    }

    default:
        break;
    }

    return 1;
}

size_t fmt::Formatter<double>::print(Context &context, double arg)
{
    char buffer[64];
    uint8_t offset;
    size_t length;
    int fill_width;
    FloatFormaterOption option;

    // Check if the argument is within the range for float type
    if (arg >= __USING_FLOAT_MIN__ && arg < __USING_FLOAT_MAX__)
    {
        option.type = FloatType::Float;
    }
    else
    {
        option.type = FloatType::Exp;
    }

    init(context, option);

    if (arg < 0)
    {
        arg = -arg;
        buffer[0] = '-';
        offset = 1;
    }
    else if (option.sign != Sign::Minus)
    {
        buffer[0] = static_cast<char>(option.sign);
        offset = 1;
    }
    else
    {
        offset = 0;
    }

    switch (option.type)
    {
    case FloatType::Exp:
    {
        char format_string[7];
        option.precision = (option.auto_precision) ? 6 : option.precision;
        snprintf(format_string, sizeof(format_string), "%%.%de", option.precision);
        snprintf(buffer + offset, sizeof(buffer) - offset, format_string, arg);
        break;
    }

    case FloatType::Float:
    {
        if (option.auto_precision)
        {
            ftoa(arg, buffer + offset);
        }
        else
        {
            ftoa(arg, buffer + offset, option.precision);
        }
        break;
    }

    default:
        break;
    }

    length = strlen(buffer);
    fill_width = (length < option.width) ? (option.width - length) : 0;

    switch (option.align)
    {
    case Align::Left:
    {
        write(buffer, length);

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

        write(buffer, length);

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

        write(buffer, length);

        break;
    }

    default:
        break;
    }

    return 1;
}
