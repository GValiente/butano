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

    [[nodiscard]] inline int lut_sin(int s16_angle)
    {
        return lu_sin(unsigned(s16_angle));
    }

    [[nodiscard]] inline int lut_cos(int s16_angle)
    {
        return lu_cos(unsigned(s16_angle));
    }
}

#endif
