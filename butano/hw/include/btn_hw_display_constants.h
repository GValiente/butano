#ifndef BTN_HW_DISPLAY_CONSTANTS_H
#define BTN_HW_DISPLAY_CONSTANTS_H

#include "btn_common.h"

namespace btn::display
{
    [[nodiscard]] constexpr int width()
    {
        return 240;
    }

    [[nodiscard]] constexpr int height()
    {
        return 160;
    }
}

#endif
