#ifndef __OLED_H__
#define __OLED_H__

#include "lib_oled.h"
#include "chinese.font"

/**
  *### Example: 
    ```
    using oled = Oled<PA<1>, PA<3>, PA<5>, PA<7>>;
    using oled = Oled<PA<1>, PA<3>, PA<5>, PA<7>, chinese_font>;
    ```
 */
using oled = Oled<PA<1>, PA<3>, PA<5>, PA<7>, chinese_font>;

#endif // !__OLED_H__
