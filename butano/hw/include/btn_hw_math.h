#ifndef BTN_HW_MATH_H
#define BTN_HW_MATH_H

#include "tonc.h"
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

    [[nodiscard]] inline int lut_sin(int lut_angle)
    {
        return sin_lut[lut_angle];
    }

    [[nodiscard]] inline int lut_cos(int lut_angle)
    {
        return sin_lut[(lut_angle + 128) & 0x1FF];
    }
}

#endif
