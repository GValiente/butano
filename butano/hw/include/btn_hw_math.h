#ifndef BTN_HW_MATH_H
#define BTN_HW_MATH_H

#include "btn_common.h"

extern "C"
{
    unsigned btn_hw_sqrt(unsigned x) BTN_CODE_IWRAM;
}

namespace btn::hw
{
    [[nodiscard]] inline int sqrt(int value)
    {
        return int(btn_hw_sqrt(unsigned(value)));
    }
}

#endif
