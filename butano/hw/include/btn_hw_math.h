#ifndef BTN_HW_MATH_H
#define BTN_HW_MATH_H

#include "btn_common.h"

namespace btn::hw
{
    [[nodiscard]] int sqrt(int value);

    [[nodiscard]] int lut_sin(int s16_angle);

    [[nodiscard]] int lut_cos(int s16_angle);
}

#endif
