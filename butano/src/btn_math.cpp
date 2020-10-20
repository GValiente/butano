/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "btn_math.h"

#include "../hw/include/btn_hw_math.h"

namespace _btn
{

int sqrt_impl(int value)
{
    return btn::hw::sqrt(value);
}

}
