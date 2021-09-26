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
        int lut_index = 0;

        for(int brightness = 0; brightness < 33; ++brightness)
        {
            for(int color = 0; color < 32; ++color)
            {
                int result = color + brightness;
                lut[lut_index] = uint8_t(bn::clamp(result, 0, 31));
                ++lut_index;
            }
        }

        return lut;
    }();

    constexpr array<uint8_t, luts_size> contrast_lut = []{
        array<uint8_t, luts_size> lut;
        int lut_index = 0;

        for(int contrast = 0; contrast < 33; ++contrast)
        {
            int contrast_256 = contrast * 8;
            int ca = contrast_256 + 256;
            int cb = (-contrast_256 >> 1) * 32;

            for(int color = 0; color < 32; ++color)
            {
                int result = (ca * color + cb) >> 8;
                lut[lut_index] = uint8_t(bn::clamp(result, 0, 31));
                ++lut_index;
            }
        }

        return lut;
    }();

    void lut_effect(const color* source_colors_ptr, const uint8_t* lut, int count, color* destination_colors_ptr)
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

void brightness(const color* source_colors_ptr, int value, int count, color* destination_colors_ptr)
{
    const uint8_t* lut = brightness_lut.data() + (value * 32);
    lut_effect(source_colors_ptr, lut, count, destination_colors_ptr);
}

void contrast(const color* source_colors_ptr, int value, int count, color* destination_colors_ptr)
{
    const uint8_t* lut = contrast_lut.data() + (value * 32);
    lut_effect(source_colors_ptr, lut, count, destination_colors_ptr);
}

}