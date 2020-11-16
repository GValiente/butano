/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_math.h"

#include "../hw/include/bn_hw_math.h"

namespace _bn
{

int sqrt_impl(int value)
{
    return bn::hw::sqrt(value);
}

}
