#include "str.hpp"

template <>
void fmt::init(Context &context, StrFormaterOption &option)
{
    auto iter = context.begin;
    option = {' ', Align::Right, 0};

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

    if (is_digit(*iter))
    {
        do
        {
            option.width = option.width * 10 + *iter++ - '0';
        } while (is_digit(*iter) && iter < context.end);
    }
}

size_t fmt::Formatter<char *>::print(fmt::Context &context, const char *arg)
{
    int fill_width;
    size_t length;
    StrFormaterOption option;

    if (arg == nullptr)
    {
        return 0;
    }

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

size_t fmt::Formatter<String>::print(fmt::Context &context, const String &arg)
{
    return Formatter<char *>::print(context, arg.c_str());
}
