#ifndef ___LIB_OLED_H__
#define __LIB_OLED_H__

#include "lib_oled.utils"

/**
 * @brief 3线 SPI Oled 模板类, 分辨率为128x64
 * 
 * @tparam sclk 时钟线(D0)(Gpio<Base, Pin>)
 * @tparam sdin 数据线(D1)(Gpio<Base, Pin>)
 * @tparam rst 复位引脚(RES)(Gpio<Base, Pin>)
 * @tparam rs 命令/数据选择引脚(DC)(Gpio<Base, Pin>)
 * @tparam chinese_font 汉字字库(ChineseFontDictionary<size>)
 * @tparam frame 用于区分界面的参数
 *
 * @note 若需要多个界面, 可以使用frame不同而其他模板参数相同的模板类
 * 
 */
template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font = __empty_chinese_font__<wchar_t, uint8_t[30]>, size_t _frame = 0>
class Oled
{
private:
    static constexpr struct
    {
        uint8_t width;
        uint8_t heigth;
    } screen = {128, 64 / 8}; // 屏幕分辨率

    static constexpr struct
    {
        uint8_t width;
        uint8_t heigth;
    } chinese = {15, 16 / 8}; // 汉字字符的大小

    using sclk = Gpio<_sclk::base, _sclk::pin>;
    using sdin = Gpio<_sdin::base, _sdin::pin>;
    using rst = Gpio<_rst::base, _rst::pin>;
    using rs = Gpio<_rs::base, _rs::pin>;
    static constexpr auto &chinese_font = _chinese_font;
    static constexpr auto frame = _frame;

    static uint8_t gram[screen.heigth][screen.width]; // Oled RAM 数据暂存区

public:
    static void init(void);
    static void pin_init(void);
    static void oled_init(void);
    static void reset(void);
    template <OledWriteType type>
    static void writebyte(uint8_t byte);

    template <OledWriteType type>
    static void writebytes(const uint8_t *bytes, size_t size);

    static void on(void);
    static void off(void);

    static void set_cursor(uint8_t page, uint8_t col);
    static void flush(void);

    static void fill(void);
    static void clear(void);

    template <OledTextSize size, OledTextBrightness brightness = OledTextBrightness::Light>
    static int put_ascii_character(uint8_t page, uint8_t col, char character);

    template <OledTextSize size, OledTextBrightness brightness = OledTextBrightness::Light>
    static int put_ascii_string(uint8_t page, uint8_t col, const char *string);

    template <OledTextSize size, OledTextBrightness brightness = OledTextBrightness::Light>
    static int put_digit(uint8_t page, uint8_t col, int digit);

    template <uint8_t base, OledTextSize size, OledTextBrightness brightness = OledTextBrightness::Light>
    static int put_digit(uint8_t page, uint8_t col, int digit);

    template <OledTextSize size, OledTextBrightness brightness = OledTextBrightness::Light>
    static int put_digit(uint8_t page, uint8_t col, double digit);

    template <OledTextSize size, OledTextBrightness brightness = OledTextBrightness::Light>
    static int put_digit(uint8_t page, uint8_t col, double digit, unsigned int round);

    template <OledTextSize size, OledTextBrightness brightness = OledTextBrightness::Light>
    static int printf(uint8_t page, uint8_t col, const char *format, ...);

    template <OledTextBrightness brightness = OledTextBrightness::Light>
    static int put_chinese_character(uint8_t page, uint8_t col, wchar_t character);

    template <OledTextBrightness brightness = OledTextBrightness::Light>
    static int put_string(uint8_t page, uint8_t col, const wchar_t *string);

    template <OledTextBrightness brightness = OledTextBrightness::Light>
    static int printf(uint8_t page, uint8_t col, const wchar_t *format, ...);
};

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
uint8_t Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::gram[screen.heigth][screen.width];

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
void Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::init(void)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");

    pin_init();
    reset();
    oled_init();
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
void Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::pin_init(void)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");

    sclk::enable_clock();
    sdin::enable_clock();
    rst::enable_clock();
    rs::enable_clock();

    sclk::odr = 1;
    sdin::odr = 1;
    rst::odr = 1;
    rs::odr = 1;

    sclk::into_output_50mhz().push_pull();
    sdin::into_output_50mhz().push_pull();
    rst::into_output_50mhz().push_pull();
    rs::into_output_50mhz().push_pull();
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
void Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::oled_init(void)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");

    writebyte<OledWriteType::Commamd>(0xAE); // 关闭显示

    writebyte<OledWriteType::Commamd>(0xD5); // 设置显示时钟分频比/振荡器频率
    writebyte<OledWriteType::Commamd>(0x80);

    writebyte<OledWriteType::Commamd>(0xA8); // 设置多路复用率
    writebyte<OledWriteType::Commamd>(0x3F);

    writebyte<OledWriteType::Commamd>(0xD3); // 设置显示偏移
    writebyte<OledWriteType::Commamd>(0x00);

    writebyte<OledWriteType::Commamd>(0x40); // 设置显示开始行

    writebyte<OledWriteType::Commamd>(0xA1); // 设置左右方向，0xA1正常 0xA0左右反置

    writebyte<OledWriteType::Commamd>(0xC8); // 设置上下方向，0xC8正常 0xC0上下反置

    writebyte<OledWriteType::Commamd>(0xDA); // 设置COM引脚硬件配置
    writebyte<OledWriteType::Commamd>(0x12);

    writebyte<OledWriteType::Commamd>(0x81); // 设置对比度控制
    writebyte<OledWriteType::Commamd>(0xCF);

    writebyte<OledWriteType::Commamd>(0xD9); // 设置预充电周期
    writebyte<OledWriteType::Commamd>(0xF1);

    writebyte<OledWriteType::Commamd>(0xDB); // 设置VCOMH取消选择级别
    writebyte<OledWriteType::Commamd>(0x30);

    writebyte<OledWriteType::Commamd>(0xA4); // 设置整个显示打开/关闭

    writebyte<OledWriteType::Commamd>(0xA6); // 设置正常/倒转显示

    writebyte<OledWriteType::Commamd>(0x8D); // 设置充电泵
    writebyte<OledWriteType::Commamd>(0x14);

    writebyte<OledWriteType::Commamd>(0xAF); // 开启显示
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
void Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::reset(void)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");

    rst::odr = 1;
    OLED_DELAY(100);
    rst::odr = 0;
    OLED_DELAY(100);
    rst::odr = 1;
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
template <OledWriteType type>
void Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::writebyte(uint8_t byte)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");
    static_assert(type == OledWriteType::Commamd || type == OledWriteType::Data,
                  "type must be OledWriteType::Commamd or OledWriteType::Data");

    rs::odr = type;
    for (size_t _ = 0; _ < 8; _++, byte <<= 1)
    {
        sclk::odr = 0;
        sdin::odr = byte >> 7;
        sclk::odr = 1;
    }
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
template <OledWriteType type>
void Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::writebytes(const uint8_t *bytes, size_t size)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");
    static_assert(type == OledWriteType::Commamd || type == OledWriteType::Data,
                  "type must be OledWriteType::Commamd or OledWriteType::Data");

    rs::odr = type;
    for (size_t i = 0; i < size; i++)
    {
        for (uint8_t msk = 1 << 7; msk != 0; msk >>= 1)
        {
            sclk::odr = 0;
            sdin::odr = (bytes[i] & msk) == msk;
            sclk::odr = 1;
        }
    }
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
void Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::on(void)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");

    writebyte<OledWriteType::Commamd>(0xAF);
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
void Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::off(void)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");

    writebyte<OledWriteType::Commamd>(0xAE);
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
void Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::set_cursor(uint8_t page, uint8_t col)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");

    writebyte<OledWriteType::Commamd>(0xB0 | (page & 0x7)); // 设置光标的页坐标
    writebyte<OledWriteType::Commamd>(0x10 | (col >> 4));   // 设置光标列坐标的高4位
    writebyte<OledWriteType::Commamd>(0x00 | (col | 0x0F)); // 设置光标列坐标的低4位
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
void Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::flush(void)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");

    for (uint8_t page = 0; page < screen.heigth; page++)
    {
        set_cursor(page, 0);
        writebytes<OledWriteType::Data>(gram[page], screen.width);
    }
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
void Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::fill(void)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");

    memset(gram, 0xFF, sizeof(gram));
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
void Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::clear(void)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");

    memset(gram, 0x00, sizeof(gram));
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
template <OledTextSize size, OledTextBrightness brightness>
int Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::put_ascii_character(uint8_t page, uint8_t col, char character)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");
    static_assert(size == OledTextSize::Pages1Cols6 || size == OledTextSize::Pages2Cols8,
                    "size must be OledTextSize::Pages1Cols6 or OledTextSize::Pages2Cols8");
    static_assert(brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark,
                    "brightness must be OledTextBrightness::Light or OledTextBrightness::Dark");

    if ((character < ' ' || character > '~') || (page >= screen.heigth || col >= screen.width))
    {
        if (character == '\n')
        {
            flush();
        }
        return 0;
    }

    character -= ' ';

    if constexpr (size == OledTextSize::Pages2Cols8 && brightness == OledTextBrightness::Light)
    {
        memcpy(gram[page] + col, font_16x8_light[(uint8_t)character], 8);
        memcpy(gram[page + 1] + col, font_16x8_light[(uint8_t)character] + 8, 8);
    }
    else if constexpr (size == OledTextSize::Pages2Cols8 && brightness == OledTextBrightness::Dark)
    {
        memcpy(gram[page] + col, font_16x8_dark[(uint8_t)character], 8);
        memcpy(gram[page + 1] + col, font_16x8_dark[(uint8_t)character] + 8, 8);
    }
    else if constexpr (size == OledTextSize::Pages1Cols6 && brightness == OledTextBrightness::Light)
    {
        memcpy(gram[page] + col, font_8x6_light[(uint8_t)character], 6);
    }
    else if constexpr (size == OledTextSize::Pages1Cols6 && brightness == OledTextBrightness::Dark)
    {
        memcpy(gram[page] + col, font_8x6_dark[(uint8_t)character], 6);
    }

    return 1;
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
template <OledTextSize size, OledTextBrightness brightness>
int Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::put_ascii_string(uint8_t page, uint8_t col, const char *string)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");
    static_assert(size == OledTextSize::Pages1Cols6 || size == OledTextSize::Pages2Cols8,
                    "size must be OledTextSize::Pages1Cols6 or OledTextSize::Pages2Cols8");
    static_assert(brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark,
                    "brightness must be OledTextBrightness::Light or OledTextBrightness::Dark");

    constexpr uint8_t text_width = size & 0xFF;
    int length = 0;

    static_assert((size == OledTextSize::Pages1Cols6 || size == OledTextSize::Pages2Cols8));
    static_assert((brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark));

    for (auto var = string; (*var != '\0') && (col + text_width <= screen.width); var++, col += text_width)
    {
        length += put_ascii_character<size, brightness>(page, col, *var);
    }

    return length;
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
template <OledTextSize size, OledTextBrightness brightness>
int Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::put_digit(uint8_t page, uint8_t col, int digit)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");
    static_assert(size == OledTextSize::Pages1Cols6 || size == OledTextSize::Pages2Cols8,
                    "size must be OledTextSize::Pages1Cols6 or OledTextSize::Pages2Cols8");
    static_assert(brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark,
                    "brightness must be OledTextBrightness::Light or OledTextBrightness::Dark");

    constexpr uint8_t text_width = size & 0xFF;
    static char buffer[screen.width / text_width + 1];
    auto var = buffer;
    int length = 0;

    static_assert((size == OledTextSize::Pages1Cols6 || size == OledTextSize::Pages2Cols8));
    static_assert((brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark));

    if (digit == 0)
    {
        return put_ascii_character<size, brightness>(page, col, '0');
    }
    else if (digit < 0)
    {
        length += put_ascii_character<size, brightness>(page, col, '-');
        if (length == 0)
            return length;

        col += text_width;
        digit = -digit;
    }

    for (; digit != 0; digit /= 10, var++)
    {
        *var = digit % 10 + '0';
    }

    var--;
    for (; (var + 1 != buffer) && (col + text_width <= screen.width); var--, col += text_width)
    {
        length += put_ascii_character<size, brightness>(page, col, *var);
    }

    return length;
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
template <uint8_t base, OledTextSize size, OledTextBrightness brightness>
int Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::put_digit(uint8_t page, uint8_t col, int digit)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");
    static_assert(size == OledTextSize::Pages1Cols6 || size == OledTextSize::Pages2Cols8,
                    "size must be OledTextSize::Pages1Cols6 or OledTextSize::Pages2Cols8");
    static_assert(brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark,
                    "brightness must be OledTextBrightness::Light or OledTextBrightness::Dark");

    constexpr uint8_t text_width = size & 0xFF;
    static char buffer[screen.width / text_width + 1];
    auto var = buffer;
    int n;
    int length = 0;

    static_assert((size == OledTextSize::Pages1Cols6 || size == OledTextSize::Pages2Cols8));
    static_assert((brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark));
    static_assert(base >= 2 && base <= 36);

    if (digit == 0)
    {
        return put_ascii_character<size, brightness>(page, col, '0');
    }
    else if (digit < 0)
    {
        length += put_ascii_character<size, brightness>(page, col, '-');
        if (length == 0)
            return length;

        col += text_width;
        digit = -digit;
    }

    for (; digit != 0; digit /= base, var++)
    {
        if constexpr (base <= 10)
        {
            *var = digit % base + '0';
        }
        else
        {
            n = digit % base;
            *var = (n < 10) ? (n + '0') : (n - 10 + 'A');
        }
    }

    var--;
    for (; (var + 1 != buffer) && (col + text_width <= screen.width); var--, col += text_width)
    {
        length += put_ascii_character<size, brightness>(page, col, *var);
    }

    return length;
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
template <OledTextSize size, OledTextBrightness brightness>
int Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::put_digit(uint8_t page, uint8_t col, double digit)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");
    static_assert(size == OledTextSize::Pages1Cols6 || size == OledTextSize::Pages2Cols8,
                    "size must be OledTextSize::Pages1Cols6 or OledTextSize::Pages2Cols8");
    static_assert(brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark,
                    "brightness must be OledTextBrightness::Light or OledTextBrightness::Dark");

    constexpr uint8_t text_width = size & 0xFF;
    int integer = (int)digit;
    double decimal = digit - integer;
    int length = 0;
    decimal = (decimal < 0) ? -decimal : decimal;

    static_assert((size == OledTextSize::Pages1Cols6 || size == OledTextSize::Pages2Cols8));
    static_assert((brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark));

    if (digit < 0. && digit > -1.)
    {
        length += put_ascii_string<size, brightness>(page, col, "-0.");

        if (length != 3)
        {
            return length;
        }
    }
    else
    {
        length = put_digit<size, brightness>(page, col, integer);

        if (col + text_width * (length + 1) > screen.width)
        {
            return length;
        }

        if (put_ascii_character<size, brightness>(page, col + length * text_width, '.') != 0)
        {
            length++;
        }
        else
        {
            return length;
        }
    }

    for (char var; decimal >= 1e-4;)
    {
        decimal *= 10;
        var = (char)decimal;
        decimal -= var;
        var += '0';
        if (1 - decimal < 1e-4)
        {
            var++;
            decimal = 0;
        }
        if (put_ascii_character<size, brightness>(page, col + length * text_width, var) != 0)
        {
            length++;
        }
        else
        {
            break;
        }
    }

    return length;
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
template <OledTextSize size, OledTextBrightness brightness>
int Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::put_digit(uint8_t page, uint8_t col, double digit, unsigned int round)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");
    static_assert(size == OledTextSize::Pages1Cols6 || size == OledTextSize::Pages2Cols8,
                    "size must be OledTextSize::Pages1Cols6 or OledTextSize::Pages2Cols8");
    static_assert(brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark,
                    "brightness must be OledTextBrightness::Light or OledTextBrightness::Dark");

    constexpr uint8_t text_width = size & 0xFF;
    int integer = (int)digit;
    double decimal = digit - integer;
    int length = 0;
    char var;
    decimal = (decimal < 0) ? -decimal : decimal;

    static_assert((size == OledTextSize::Pages1Cols6 || size == OledTextSize::Pages2Cols8));
    static_assert((brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark));

    if (digit < 0. && digit > -1.)
    {
        length += put_ascii_string<size, brightness>(page, col, "-0.");

        if (length != 3)
        {
            return length;
        }
    }
    else
    {
        length = put_digit<size, brightness>(page, col, integer);

        if (col + text_width * (length + 1) > screen.width)
        {
            return length;
        }

        if (put_ascii_character<size, brightness>(page, col + length * text_width, '.') != 0)
        {
            length++;
        }
        else
        {
            return length;
        }
    }

    for (unsigned int i = 0; i < round; i++)
    {
        decimal *= 10;
        var = (char)decimal;
        decimal -= var;
        var += '0';
        if (1 - decimal < 1e-4)
        {
            var++;
            decimal = 0;
        }
        if (put_ascii_character<size, brightness>(page, col + length * text_width, var) != 0)
        {
            length++;
        }
        else
        {
            break;
        }
    }

    return length;
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
template <OledTextSize size, OledTextBrightness brightness>
int Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::printf(uint8_t page, uint8_t col, const char *format, ...)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");
    static_assert(size == OledTextSize::Pages1Cols6 || size == OledTextSize::Pages2Cols8,
                    "size must be OledTextSize::Pages1Cols6 or OledTextSize::Pages2Cols8");
    static_assert(brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark,
                    "brightness must be OledTextBrightness::Light or OledTextBrightness::Dark");

    va_list v;
    constexpr uint8_t text_width = size & 0xFF;
    static char buffer[screen.width / text_width + 1];

    va_start(v, format);
    vsnprintf(buffer, sizeof(buffer), format, v);
    va_end(v);

    return put_ascii_string<size, brightness>(page, col, buffer);
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
template <OledTextBrightness brightness>
int Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::put_chinese_character(uint8_t page, uint8_t col, wchar_t character)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");
    static_assert(brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark,
                    "brightness must be OledTextBrightness::Light or OledTextBrightness::Dark");

    if constexpr (brightness == OledTextBrightness::Light)
    {
        auto &mat = chinese_font[character];
        memcpy(gram[page] + col, mat, chinese.width);
        memcpy(gram[page + 1] + col, mat + chinese.width, chinese.width);
    }
    else
    {
        auto &mat = chinese_font.inv()[character];
        memcpy(gram[page] + col, mat, chinese.width);
        memcpy(gram[page + 1] + col, mat + chinese.width, chinese.width);
    }
    
    return 1;
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
template <OledTextBrightness brightness>
int Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::put_string(uint8_t page, uint8_t col, const wchar_t *string)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");
    static_assert(brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark,
                    "brightness must be OledTextBrightness::Light or OledTextBrightness::Dark");

    constexpr uint8_t ascii_text_width = OledTextSize::Pages2Cols8 & 0xFF;
    auto *var = string;
    int length = 0;

    static_assert(brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark);

    for (; *var != L'\0'; var++)
    {
        if (*var <= L'~')
        {
            if (col + ascii_text_width > screen.width)
            {
                return length;
            }
            length += put_ascii_character<OledTextSize::Pages2Cols8, brightness>(page, col, (char)*var);
            col += ascii_text_width;
        }
        else
        {
            if (col + chinese.width > screen.width)
            {
                return length;
            }
            length += put_chinese_character<brightness>(page, col, *var);
            col += chinese.width;
        }
    }

    return length;
}

template <class _sclk, class _sdin, class _rst, class _rs, const auto &_chinese_font, size_t _frame>
template <OledTextBrightness brightness>
int Oled<_sclk, _sdin, _rst, _rs, _chinese_font, _frame>::printf(uint8_t page, uint8_t col, const wchar_t *format, ...)
{
    static_assert(is_specialization_of_gpio<sclk>::value, "sclk must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<sdin>::value, "sdin must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rst>::value, "rst must be the specialization of Gpio");
    static_assert(is_specialization_of_gpio<rs>::value, "rs must be the specialization of Gpio");
    static_assert(brightness == OledTextBrightness::Light || brightness == OledTextBrightness::Dark,
                    "brightness must be OledTextBrightness::Light or OledTextBrightness::Dark");

    va_list v;
    constexpr uint8_t ascii_text_width = OledTextSize::Pages2Cols8 & 0xFF;
    static wchar_t buffer[screen.width / ascii_text_width + 1];

    va_start(v, format);
    vswprintf(buffer, sizeof(buffer), format, v);
    va_end(v);

    return put_string<brightness>(page, col, buffer);
}

#endif // !___LIB_OLED_H__
