/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_div_lut.h"

#include "bn_array.h"

namespace fr
{

namespace
{
    constexpr int div_lut_size = 1024 * 4;

    constexpr bn::array<uint32_t, div_lut_size> div_lut = []{
        bn::array<uint32_t, div_lut_size> result;

        for(int index = 0; index < div_lut_size; ++index)
        {
            result[index] = calculate_div_lut_value(index);
        }

        return result;
    }();
}

const uint32_t* div_lut_ptr = div_lut._data;

}
