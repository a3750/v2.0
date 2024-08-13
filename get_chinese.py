'''
此段代码可用于取汉字字模
在工作区的根目录下必须有 oled_font_gb2312 文件
'''
words = "零一二三四五六七八九"

import binascii

file_path = "oled_font_gb2312"

with open("oled/chinese.font", "w") as chinese_font:
    chinese_font.write("#ifndef __CHINESE_FONT__\n")
    chinese_font.write("#define __CHINESE_FONT__\n")
    chinese_font.write("\n")
    chinese_font.write('#include "chinese.h"\n')
    chinese_font.write("\n")
    chinese_font.write("static constexpr inline Dictionary chinese_font = Dictionary(\n")

    for word in words:
        gb2312 = word.encode("gb2312")
        hex_str = binascii.b2a_hex(gb2312)
        result = str(hex_str, encoding="utf-8")
        area = eval("0x" + result[:2]) - 0xA0
        index = eval("0x" + result[2:]) - 0xA0
        offset = (94 * (area - 1) + (index - 1)) * 30
        with open(file_path, "rb") as f:
            f.seek(offset)
            data = f.read(30)
            chinese_font.write(f"    KeyValuePairs<wchar_t, uint8_t[30]>{{L'{word}', {{{', '.join([f'0x{i:02x}' for i in data])}}}}},\n")

    chinese_font.seek(chinese_font.tell() - 2)
    chinese_font.write("\n")
    chinese_font.write(");\n")
    chinese_font.write("\n")
    chinese_font.write("#endif // !__CHINESE_FONT__\n")
