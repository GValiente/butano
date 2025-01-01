/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_palettes.h"

#include "bn_algorithm.h"

namespace bn::hw::palettes
{

void brightness(const color* source_colors_ptr, int value, int count, color* destination_colors_ptr)
{
    auto tonc_dst_ptr = reinterpret_cast<COLOR*>(destination_colors_ptr);

    for(int index = count - 1; index >= 0; --index)
    {
        color color = source_colors_ptr[index];
        int red = bn::min(color.red() + value, 31);
        int green = bn::min(color.green() + value, 31);
        int blue = bn::min(color.blue() + value, 31);
        tonc_dst_ptr[index] = RGB15(red, green, blue);
    }
}

void _lut_effect(const color* source_colors_ptr, const uint8_t* lut, int count, color* destination_colors_ptr)
{
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
