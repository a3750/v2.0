#include "int.hpp"

template <>
void fmt::init(Context &context, IntFormaterOption &option)
{
    auto iter = context.begin;
    option = {' ', Align::Right, Sign::Minus, 0, option.type};

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

    if (is_int_type(*iter))
    {
        option.type = static_cast<IntType>(*iter);
    }
}

String fmt::Formatter<bool>::format(fmt::Context &context, bool arg)
{
    String result;
    int fill_width;
    IntFormaterOption option;

    option.type = IntType::Bol;
    init(context, option) ;

    if (option.type == IntType::Chr)
    {
        return result;
    }

    if (option.type == IntType::Bol)
    {
        result = arg ? "true" : "false";
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            result = option.sign;
            result += (char)(arg + '0');
            break;
        case Sign::Minus:
            result = (char)(arg + '0');
            break;
        default:
            break;
        }
    }

    fill_width = (option.width > result.length()) ? (option.width - result.length()) : 0;

    switch (option.align)
    {
    case Align::Left:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result += option.fill;
        }
        break;
    }

    case Align::Centering:
    {
        for (int i = 0; i < fill_width / 2; i++)
        {
            result = option.fill + result;
        }

        for (int i = 0; i < (fill_width + 1) / 2; i++)
        {
            result += option.fill;
        }

        break;
    }
    case Align::Right:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result = option.fill + result;
        }

        break;
    }
    
    default:
        break;
    }

    return result;
}

size_t fmt::Formatter<bool>::print(fmt::Context &context, bool arg)
{
    char buffer[5];
    int fill_width;
    size_t length = 0;
    IntFormaterOption option;

    option.type = IntType::Bol;
    init(context, option) ;

    if (option.type == IntType::Chr)
    {
        return 0;
    }

    if (option.type == IntType::Bol)
    {
        length = arg ? 4 : 5;
        memcpy(buffer, arg ? "true" : "false", length + 1);
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            buffer[0] = option.sign;
            buffer[1] = arg + '0';
            buffer[2] = '\0';
            length = 2;
            break;
        case Sign::Minus:
            buffer[0] = arg + '0';
            buffer[1] = '\0';
            length = 1;
            break;
        default:
            break;
        }
    }

    fill_width = (option.width > length) ? (option.width - length) : 0;

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

String fmt::Formatter<char>::format(fmt::Context &context, char arg)
{
    String result;
    int base;
    int fill_width;
    IntFormaterOption option;

    option.type = IntType::Chr;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        result = arg ? "true" : "false";
    }
    else if (option.type == IntType::Chr)
    {
        result += arg;
    }
    else if (arg < 0)
    {
        result = (String &&)String(arg, base);
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            result = (String &&)(option.sign + String(arg, base));
            break;
        case Sign::Minus:
            result = (String &&)String(arg, base);
            break;
        default:
            break;
        }
    }

    fill_width = (option.width > result.length()) ? (option.width - result.length()) : 0;

    if (option.type == IntType::Hex)
    {
        result.toUpperCase();
    }

    switch (option.align)
    {
    case Align::Left:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result += option.fill;
        }

        break;
    }

    case Align::Centering:
    {
        for (int i = 0; i < fill_width / 2; i++)
        {
            result = option.fill + result;
        }

        for (int i = 0; i < (fill_width + 1) / 2; i++)
        {
            result += option.fill;
        }

        break;
    }
    case Align::Right:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result = option.fill + result;
        }

        break;
    }
    
    default:
        break;
    }

    return result;
}

size_t fmt::Formatter<char>::print(fmt::Context &context, char arg)
{
    char buffer[25];
    int base;
    int fill_width;
    size_t length;
    IntFormaterOption option;

    option.type = IntType::Chr;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        strcpy(buffer, arg ? "true" : "false");
    }
    else if (option.type == IntType::Chr)
    {
        buffer[0] = arg;
        buffer[1] = '\0';
    }
    else if (arg < 0)
    {
        itoa(arg, buffer, base);
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            buffer[0] = option.sign;
            itoa(arg, buffer + 1, base);
            break;
        case Sign::Minus:
            itoa(arg, buffer, base);
            break;
        default:
            break;
        }
    }

    length = strlen(buffer);
    fill_width = (option.width > length) ? (option.width - length) : 0;

    for (size_t i = 0; i < length && option.type == IntType::Hex; i++)
    {
        if (buffer[i] <= 'z' && buffer[i] >= 'a')
        {
            buffer[i] -= 'a' - 'A';
        }
    }

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

String fmt::Formatter<signed char>::format(fmt::Context &context, signed char arg)
{
    String result;
    int base;
    int fill_width;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        result = arg ? "true" : "false";
    }
    else if (option.type == IntType::Chr)
    {
        result += (char)arg;
    }
    else if (arg < 0)
    {
        result = String(arg, base);
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            result += option.sign;
            result += String(arg, base);
            break;
        case Sign::Minus:
            result = String(arg, base);
            break;
        default:
            break;
        }
    }

    fill_width = (option.width > result.length()) ? (option.width - result.length()) : 0;

    if (option.type == IntType::Hex)
    {
        result.toUpperCase();
    }

    switch (option.align)
    {
    case Align::Left:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result += option.fill;
        }

        break;
    }

    case Align::Centering:
    {
        for (int i = 0; i < fill_width / 2; i++)
        {
            result = option.fill + result;
        }

        for (int i = 0; i < (fill_width + 1) / 2; i++)
        {
            result += option.fill;
        }

        break;
    }
    case Align::Right:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result = option.fill + result;
        }

        break;
    }
    
    default:
        break;
    }

    return result;
}

size_t fmt::Formatter<signed char>::print(fmt::Context &context, signed char arg)
{
    char buffer[25];
    int base;
    int fill_width;
    size_t length;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        strcpy(buffer, arg ? "true" : "false");
    }
    else if (option.type == IntType::Chr)
    {
        buffer[0] = arg;
        buffer[1] = '\0';
    }
    else if (arg < 0)
    {
        itoa(arg, buffer, base);
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            buffer[0] = option.sign;
            itoa(arg, buffer + 1, base);
            break;
        case Sign::Minus:
            itoa(arg, buffer, base);
            break;
        default:
            break;
        }
    }

    length = strlen(buffer);
    fill_width = (option.width > length) ? (option.width - length) : 0;

    for (size_t i = 0; i < length && option.type == IntType::Hex; i++)
    {
        if (buffer[i] <= 'z' && buffer[i] >= 'a')
        {
            buffer[i] -= 'a' - 'A';
        }
    }

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

String fmt::Formatter<unsigned char>::format(fmt::Context &context, unsigned char arg)
{
    String result;
    int base;
    int fill_width;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        result = arg ? "true" : "false";
    }
    else if (option.type == IntType::Chr)
    {
        result += (char)arg;
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            result += (char)option.sign;
            result += String(arg, base);
            break;
        case Sign::Minus:
            result = String(arg, base);
            break;
        default:
            break;
        }
    }

    fill_width = (option.width > result.length()) ? (option.width - result.length()) : 0;

    if (option.type == IntType::Hex)
    {
        result.toUpperCase();
    }

    switch (option.align)
    {
    case Align::Left:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result += option.fill;
        }

        break;
    }

    case Align::Centering:
    {
        for (int i = 0; i < fill_width / 2; i++)
        {
            result = option.fill + result;
        }

        for (int i = 0; i < (fill_width + 1) / 2; i++)
        {
            result += option.fill;
        }

        break;
    }
    case Align::Right:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result = option.fill + result;
        }

        break;
    }
    
    default:
        break;
    }

    return result;
}

size_t fmt::Formatter<unsigned char>::print(fmt::Context &context, unsigned char arg)
{
    char buffer[25];
    int base;
    int fill_width;
    size_t length;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        strcpy(buffer, arg ? "true" : "false");
    }
    else if (option.type == IntType::Chr)
    {
        buffer[0] = arg;
        buffer[1] = '\0';
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            buffer[0] = option.sign;
            itoa(arg, buffer + 1, base);
            break;
        case Sign::Minus:
            itoa(arg, buffer, base);
            break;
        default:
            break;
        }
    }

    length = strlen(buffer);
    fill_width = (option.width > length) ? (option.width - length) : 0;

    for (size_t i = 0; i < length && option.type == IntType::Hex; i++)
    {
        if (buffer[i] <= 'z' && buffer[i] >= 'a')
        {
            buffer[i] -= 'a' - 'A';
        }
    }

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

String fmt::Formatter<signed short>::format(fmt::Context &context, signed short arg)
{
    String result;
    int base;
    int fill_width;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        result = arg ? "true" : "false";
    }
    else if (option.type == IntType::Chr)
    {
        result += (char)arg;
    }
    else if (arg < 0)
    {
        result = String(arg, base);
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            result += option.sign;
            result += String(arg, base);
            break;
        case Sign::Minus:
            result = String(arg, base);
            break;
        default:
            break;
        }
    }

    fill_width = (option.width > result.length()) ? (option.width - result.length()) : 0;

    if (option.type == IntType::Hex)
    {
        result.toUpperCase();
    }

    switch (option.align)
    {
    case Align::Left:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result += option.fill;
        }

        break;
    }

    case Align::Centering:
    {
        for (int i = 0; i < fill_width / 2; i++)
        {
            result = option.fill + result;
        }

        for (int i = 0; i < (fill_width + 1) / 2; i++)
        {
            result += option.fill;
        }

        break;
    }
    case Align::Right:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result = option.fill + result;
        }

        break;
    }
    
    default:
        break;
    }

    return result;
}

size_t fmt::Formatter<signed short>::print(fmt::Context &context, signed short arg)
{
    char buffer[25];
    int base;
    int fill_width;
    size_t length;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        strcpy(buffer, arg ? "true" : "false");
    }
    else if (option.type == IntType::Chr)
    {
        buffer[0] = arg;
        buffer[1] = '\0';
    }
    else if (arg < 0)
    {
        itoa(arg, buffer, base);
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            buffer[0] = option.sign;
            itoa(arg, buffer + 1, base);
            break;
        case Sign::Minus:
            itoa(arg, buffer, base);
            break;
        default:
            break;
        }
    }

    length = strlen(buffer);
    fill_width = (option.width > length) ? (option.width - length) : 0;

    for (size_t i = 0; i < length && option.type == IntType::Hex; i++)
    {
        if (buffer[i] <= 'z' && buffer[i] >= 'a')
        {
            buffer[i] -= 'a' - 'A';
        }
    }

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

String fmt::Formatter<unsigned short>::format(fmt::Context &context, unsigned short arg)
{
    String result;
    int base;
    int fill_width;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        result = arg ? "true" : "false";
    }
    else if (option.type == IntType::Chr)
    {
        result += (char)arg;
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            result += (char)option.sign;
            result += String(arg, base);
            break;
        case Sign::Minus:
            result = String(arg, base);
            break;
        default:
            break;
        }
    }

    fill_width = (option.width > result.length()) ? (option.width - result.length()) : 0;

    if (option.type == IntType::Hex)
    {
        result.toUpperCase();
    }

    switch (option.align)
    {
    case Align::Left:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result += option.fill;
        }

        break;
    }

    case Align::Centering:
    {
        for (int i = 0; i < fill_width / 2; i++)
        {
            result = option.fill + result;
        }

        for (int i = 0; i < (fill_width + 1) / 2; i++)
        {
            result += option.fill;
        }

        break;
    }
    case Align::Right:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result = option.fill + result;
        }

        break;
    }
    
    default:
        break;
    }

    return result;
}

size_t fmt::Formatter<unsigned short>::print(fmt::Context &context, unsigned short arg)
{
    char buffer[25];
    int base;
    int fill_width;
    size_t length;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        strcpy(buffer, arg ? "true" : "false");
    }
    else if (option.type == IntType::Chr)
    {
        buffer[0] = arg;
        buffer[1] = '\0';
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            buffer[0] = option.sign;
            itoa(arg, buffer + 1, base);
            break;
        case Sign::Minus:
            itoa(arg, buffer, base);
            break;
        default:
            break;
        }
    }

    length = strlen(buffer);
    fill_width = (option.width > length) ? (option.width - length) : 0;

    for (size_t i = 0; i < length && option.type == IntType::Hex; i++)
    {
        if (buffer[i] <= 'z' && buffer[i] >= 'a')
        {
            buffer[i] -= 'a' - 'A';
        }
    }

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

String fmt::Formatter<signed int>::format(fmt::Context &context, signed int arg)
{
    String result;
    int base;
    int fill_width;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        result = arg ? "true" : "false";
    }
    else if (option.type == IntType::Chr)
    {
        result += (char)arg;
    }
    else if (arg < 0)
    {
        result = String(arg, base);
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            result += option.sign;
            result += String(arg, base);
            break;
        case Sign::Minus:
            result = String(arg, base);
            break;
        default:
            break;
        }
    }

    fill_width = (option.width > result.length()) ? (option.width - result.length()) : 0;

    if (option.type == IntType::Hex)
    {
        result.toUpperCase();
    }

    switch (option.align)
    {
    case Align::Left:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result += option.fill;
        }

        break;
    }

    case Align::Centering:
    {
        for (int i = 0; i < fill_width / 2; i++)
        {
            result = option.fill + result;
        }

        for (int i = 0; i < (fill_width + 1) / 2; i++)
        {
            result += option.fill;
        }

        break;
    }
    case Align::Right:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result = option.fill + result;
        }

        break;
    }
    
    default:
        break;
    }

    return result;
}

size_t fmt::Formatter<signed int>::print(fmt::Context &context, signed int arg)
{
    char buffer[25];
    int base;
    int fill_width;
    size_t length;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        strcpy(buffer, arg ? "true" : "false");
    }
    else if (option.type == IntType::Chr)
    {
        buffer[0] = arg;
        buffer[1] = '\0';
    }
    else if (arg < 0)
    {
        itoa(arg, buffer, base);
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            buffer[0] = option.sign;
            itoa(arg, buffer + 1, base);
            break;
        case Sign::Minus:
            itoa(arg, buffer, base);
            break;
        default:
            break;
        }
    }

    length = strlen(buffer);
    fill_width = (option.width > length) ? (option.width - length) : 0;

    for (size_t i = 0; i < length && option.type == IntType::Hex; i++)
    {
        if (buffer[i] <= 'z' && buffer[i] >= 'a')
        {
            buffer[i] -= 'a' - 'A';
        }
    }

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

String fmt::Formatter<unsigned int>::format(fmt::Context &context, unsigned int arg)
{
    String result;
    int base;
    int fill_width;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        result = arg ? "true" : "false";
    }
    else if (option.type == IntType::Chr)
    {
        result += (char)arg;
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            result += (char)option.sign;
            result += String(arg, base);
            break;
        case Sign::Minus:
            result = String(arg, base);
            break;
        default:
            break;
        }
    }

    fill_width = (option.width > result.length()) ? (option.width - result.length()) : 0;

    if (option.type == IntType::Hex)
    {
        result.toUpperCase();
    }

    switch (option.align)
    {
    case Align::Left:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result += option.fill;
        }

        break;
    }

    case Align::Centering:
    {
        for (int i = 0; i < fill_width / 2; i++)
        {
            result = option.fill + result;
        }

        for (int i = 0; i < (fill_width + 1) / 2; i++)
        {
            result += option.fill;
        }

        break;
    }
    case Align::Right:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result = option.fill + result;
        }

        break;
    }
    
    default:
        break;
    }

    return result;
}

size_t fmt::Formatter<unsigned int>::print(fmt::Context &context, unsigned int arg)
{
    char buffer[25];
    int base;
    int fill_width;
    size_t length;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        strcpy(buffer, arg ? "true" : "false");
    }
    else if (option.type == IntType::Chr)
    {
        buffer[0] = arg;
        buffer[1] = '\0';
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            buffer[0] = option.sign;
            itoa(arg, buffer + 1, base);
            break;
        case Sign::Minus:
            itoa(arg, buffer, base);
            break;
        default:
            break;
        }
    }

    length = strlen(buffer);
    fill_width = (option.width > length) ? (option.width - length) : 0;

    for (size_t i = 0; i < length && option.type == IntType::Hex; i++)
    {
        if (buffer[i] <= 'z' && buffer[i] >= 'a')
        {
            buffer[i] -= 'a' - 'A';
        }
    }

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

String fmt::Formatter<signed long>::format(fmt::Context &context, signed long arg)
{
    String result;
    int base;
    int fill_width;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        result = arg ? "true" : "false";
    }
    else if (option.type == IntType::Chr)
    {
        result += (char)arg;
    }
    else if (arg < 0)
    {
        result = String(arg, base);
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            result += option.sign;
            result += String(arg, base);
            break;
        case Sign::Minus:
            result = String(arg, base);
            break;
        default:
            break;
        }
    }

    fill_width = (option.width > result.length()) ? (option.width - result.length()) : 0;

    if (option.type == IntType::Hex)
    {
        result.toUpperCase();
    }

    switch (option.align)
    {
    case Align::Left:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result += option.fill;
        }

        break;
    }

    case Align::Centering:
    {
        for (int i = 0; i < fill_width / 2; i++)
        {
            result = option.fill + result;
        }

        for (int i = 0; i < (fill_width + 1) / 2; i++)
        {
            result += option.fill;
        }

        break;
    }
    case Align::Right:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result = option.fill + result;
        }

        break;
    }
    
    default:
        break;
    }

    return result;
}

size_t fmt::Formatter<signed long>::print(fmt::Context &context, signed long arg)
{
    char buffer[25];
    int base;
    int fill_width;
    size_t length;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        strcpy(buffer, arg ? "true" : "false");
    }
    else if (option.type == IntType::Chr)
    {
        buffer[0] = arg;
        buffer[1] = '\0';
    }
    else if (arg < 0)
    {
        ltoa(arg, buffer, base);
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
            buffer[0] = ' ';
            ltoa(arg, buffer + 1, base);
            break;
        case Sign::Plus:
            buffer[0] = '+';
            ltoa(arg, buffer + 1, base);
            break;
        case Sign::Minus:
            ltoa(arg, buffer, base);
            break;
        default:
            break;
        }
    }

    length = strlen(buffer);
    fill_width = (option.width > length) ? (option.width - length) : 0;

    for (size_t i = 0; i < length && option.type == IntType::Hex; i++)
    {
        if (buffer[i] <= 'z' && buffer[i] >= 'a')
        {
            buffer[i] -= 'a' - 'A';
        }
    }

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

String fmt::Formatter<unsigned long>::format(fmt::Context &context, unsigned long arg)
{
    String result;
    int base;
    int fill_width;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        result = arg ? "true" : "false";
    }
    else if (option.type == IntType::Chr)
    {
        result += (char)arg;
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            result += (char)option.sign;
            result += String(arg, base);
            break;
        case Sign::Minus:
            result = String(arg, base);
            break;
        default:
            break;
        }
    }

    fill_width = (option.width > result.length()) ? (option.width - result.length()) : 0;

    if (option.type == IntType::Hex)
    {
        result.toUpperCase();
    }

    switch (option.align)
    {
    case Align::Left:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result += option.fill;
        }

        break;
    }

    case Align::Centering:
    {
        for (int i = 0; i < fill_width / 2; i++)
        {
            result = option.fill + result;
        }

        for (int i = 0; i < (fill_width + 1) / 2; i++)
        {
            result += option.fill;
        }

        break;
    }
    case Align::Right:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result = option.fill + result;
        }

        break;
    }
    
    default:
        break;
    }

    return result;
}

size_t fmt::Formatter<unsigned long>::print(fmt::Context &context, unsigned long arg)
{
    char buffer[25];
    int base;
    int fill_width;
    size_t length;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        base = 10;
        break;
    }

    switch (option.sign)
    {
    case Sign::Space:
        buffer[0] = ' ';
        ltoa(arg, buffer + 1, base);
        break;
    case Sign::Plus:
        buffer[0] = '+';
        ltoa(arg, buffer + 1, base);
        break;
    case Sign::Minus:
        ltoa(arg, buffer, base);
        break;
    default:
        break;
    }

    length = strlen(buffer);
    fill_width = (option.width > length) ? (option.width - length) : 0;

    for (size_t i = 0; i < length && option.type == IntType::Hex; i++)
    {
        if (buffer[i] <= 'z' && buffer[i] >= 'a')
        {
            buffer[i] -= 'a' - 'A';
        }
    }

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

String fmt::Formatter<signed long long>::format(fmt::Context &context, signed long long arg)
{
    String result;
    int base;
    int fill_width;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        result = arg ? "true" : "false";
    }
    else if (option.type == IntType::Chr)
    {
        result += (char)arg;
    }
    else if (arg < 0)
    {
        result = String((unsigned long)arg, base);
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            result += option.sign;
            result += String((unsigned long)arg, base);
            break;
        case Sign::Minus:
            result = String((unsigned long)arg, base);
            break;
        default:
            break;
        }
    }

    fill_width = (option.width > result.length()) ? (option.width - result.length()) : 0;

    if (option.type == IntType::Hex)
    {
        result.toUpperCase();
    }

    switch (option.align)
    {
    case Align::Left:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result += option.fill;
        }

        break;
    }

    case Align::Centering:
    {
        for (int i = 0; i < fill_width / 2; i++)
        {
            result = option.fill + result;
        }

        for (int i = 0; i < (fill_width + 1) / 2; i++)
        {
            result += option.fill;
        }

        break;
    }
    case Align::Right:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result = option.fill + result;
        }

        break;
    }
    
    default:
        break;
    }

    return result;
}

size_t fmt::Formatter<signed long long>::print(fmt::Context &context, signed long long arg)
{
    char buffer[25];
    int base;
    int fill_width;
    size_t length;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        strcpy(buffer, arg ? "true" : "false");
    }
    else if (option.type == IntType::Chr)
    {
        buffer[0] = arg;
        buffer[1] = '\0';
    }
    else if (arg < 0)
    {
        ltoa(arg, buffer, base);
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
            buffer[0] = ' ';
            ltoa(arg, buffer + 1, base);
            break;
        case Sign::Plus:
            buffer[0] = '+';
            ltoa(arg, buffer + 1, base);
            break;
        case Sign::Minus:
            ltoa(arg, buffer, base);
            break;
        default:
            break;
        }
    }

    length = strlen(buffer);
    fill_width = (option.width > length) ? (option.width - length) : 0;

    for (size_t i = 0; i < length && option.type == IntType::Hex; i++)
    {
        if (buffer[i] <= 'z' && buffer[i] >= 'a')
        {
            buffer[i] -= 'a' - 'A';
        }
    }

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

String fmt::Formatter<unsigned long long>::format(fmt::Context &context, unsigned long long arg)
{
    String result;
    int base;
    int fill_width;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        result = arg ? "true" : "false";
    }
    else if (option.type == IntType::Chr)
    {
        result += (char)arg;
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
        case Sign::Plus:
            result += (char)option.sign;
            result += String((unsigned long)arg, base);
            break;
        case Sign::Minus:
            result = String((unsigned long)arg, base);
            break;
        default:
            break;
        }
    }

    fill_width = (option.width > result.length()) ? (option.width - result.length()) : 0;

    if (option.type == IntType::Hex)
    {
        result.toUpperCase();
    }

    switch (option.align)
    {
    case Align::Left:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result += option.fill;
        }

        break;
    }

    case Align::Centering:
    {
        for (int i = 0; i < fill_width / 2; i++)
        {
            result = option.fill + result;
        }

        for (int i = 0; i < (fill_width + 1) / 2; i++)
        {
            result += option.fill;
        }

        break;
    }
    case Align::Right:
    {
        for (int i = 0; i < fill_width; i++)
        {
            result = option.fill + result;
        }

        break;
    }
    
    default:
        break;
    }

    return result;
}

size_t fmt::Formatter<unsigned long long>::print(fmt::Context &context, unsigned long long arg)
{
    char buffer[25];
    int base;
    int fill_width;
    size_t length;
    IntFormaterOption option;

    option.type = IntType::Dec;
    init(context, option);

    switch (option.type)
    {
    case IntType::Bin:
        base = 2;
        break;
    case IntType::Oct:
        base = 8;
        break;
    case IntType::Hex:
    case IntType::hex:
        base = 16;
        break;
    case IntType::Dec:
    default:
        base = 10;
        base = 10;
        break;
    }

    if (option.type == IntType::Bol)
    {
        strcpy(buffer, arg ? "true" : "false");
    }
    else if (option.type == IntType::Chr)
    {
        buffer[0] = arg;
        buffer[1] = '\0';
    }
    else
    {
        switch (option.sign)
        {
        case Sign::Space:
            buffer[0] = ' ';
            ltoa(arg, buffer + 1, base);
            break;
        case Sign::Plus:
            buffer[0] = '+';
            ltoa(arg, buffer + 1, base);
            break;
        case Sign::Minus:
            ltoa(arg, buffer, base);
            break;
        default:
            break;
        }
    }

    length = strlen(buffer);
    fill_width = (option.width > length) ? (option.width - length) : 0;

    for (size_t i = 0; i < length && option.type == IntType::Hex; i++)
    {
        if (buffer[i] <= 'z' && buffer[i] >= 'a')
        {
            buffer[i] -= 'a' - 'A';
        }
    }

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
