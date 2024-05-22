/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "fr_sin_cos.h"

#include "bn_array.h"

namespace fr
{

namespace
{
    constexpr bn::array<int16_t, 65536> sin_lut = []{
        bn::array<int16_t, 65536> result;

        for(int index = 0; index < 65536; ++index)
        {
            result[index] = int16_t(sin(index).data());
        }

        return result;
    }();
}

const int16_t* sin_lut_ptr = sin_lut._data;

}
