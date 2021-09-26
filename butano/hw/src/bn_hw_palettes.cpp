/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_palettes.h"

#include "bn_array.h"

namespace bn::hw::palettes
{

namespace
{
    constexpr int luts_size = 33 * 32;

    constexpr array<uint8_t, luts_size> brightness_lut = []{
        array<uint8_t, luts_size> lut;

        for(int index = 0; index < luts_size; ++index)
        {
            int brightness = index / 32;
            int color = index % 32;
            int result = bn::min(color + brightness, 31);
            lut[index] = uint8_t(result);
        }

        return lut;
    }();
}

void brightness(const color* source_colors_ptr, int value, int count, color* destination_colors_ptr)
{
    const uint8_t* lut = brightness_lut.data() + (value * 32);
    auto tonc_dst_ptr = reinterpret_cast<COLOR*>(destination_colors_ptr);

    for(int index = 0; index < count; ++index)
    {
        color color = source_colors_ptr[index];
        int red = lut[color.red()];
        int green = lut[color.green()];
        int blue = lut[color.blue()];
        tonc_dst_ptr[index] = RGB15(red, green, blue);
    }
}

}
