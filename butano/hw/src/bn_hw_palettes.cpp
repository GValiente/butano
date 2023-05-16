/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "../include/bn_hw_palettes.h"

#include "bn_math.h"

namespace bn::hw::palettes
{

namespace
{
    constexpr int luts_size = 33 * 32;

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

    constexpr array<uint8_t, luts_size> intensity_lut = []{
        array<uint8_t, luts_size> lut;
        int lut_index = 0;

        for(int intensity = 0; intensity < 33; ++intensity)
        {
            int intensity_256 = intensity * 8;
            int ia = intensity_256 + 256;

            for(int color = 0; color < 32; ++color)
            {
                int result = (ia * color) >> 8;
                lut[lut_index] = uint8_t(bn::clamp(result, 0, 31));
                ++lut_index;
            }
        }

        return lut;
    }();

    constexpr int hue_shift_lut_size = 33 * 9;

    constexpr array<fixed, hue_shift_lut_size> hue_shift_lut = []{
        fixed f13 = fixed(1) / 3;
        fixed f13_sqrt = sqrt(f13);

        array<fixed, hue_shift_lut_size> lut;
        int lut_index = 0;

        for(int intensity = 0; intensity < 33; ++intensity)
        {
            fixed_t<16> angle = fixed(intensity) / 32;
            fixed sin = bn::sin(angle);
            fixed cos = bn::cos(angle);

            lut[lut_index] = cos + (1 - cos) / 3;
            ++lut_index;

            lut[lut_index] = f13 * (1 - cos) - f13_sqrt * sin;
            ++lut_index;

            lut[lut_index] = f13 * (1 - cos) + f13_sqrt * sin;
            ++lut_index;

            lut[lut_index] = f13 * (1 - cos) + f13_sqrt * sin;
            ++lut_index;

            lut[lut_index] = cos + f13 * (1 - cos);
            ++lut_index;

            lut[lut_index] = f13 * (1 - cos) - f13_sqrt * sin;
            ++lut_index;

            lut[lut_index] = f13 * (1 - cos) - f13_sqrt * sin;
            ++lut_index;

            lut[lut_index] = f13 * (1 - cos) + f13_sqrt * sin;
            ++lut_index;

            lut[lut_index] = cos + f13 * (1 - cos);
            ++lut_index;
        }

        return lut;
    }();
}

void contrast(const color* source_colors_ptr, int value, int count, color* destination_colors_ptr)
{
    const uint8_t* lut = contrast_lut.data() + (value * 32);
    _lut_effect(source_colors_ptr, lut, count, destination_colors_ptr);
}

void intensity(const color* source_colors_ptr, int value, int count, color* destination_colors_ptr)
{
    const uint8_t* lut = intensity_lut.data() + (value * 32);
    _lut_effect(source_colors_ptr, lut, count, destination_colors_ptr);
}

void hue_shift(const color* source_colors_ptr, int value, int count, color* destination_colors_ptr)
{
    const fixed* lut = hue_shift_lut.data() + (value * 9);
    auto tonc_dst_ptr = reinterpret_cast<COLOR*>(destination_colors_ptr);

    for(int index = 0; index < count; ++index)
    {
        color color = source_colors_ptr[index];
        int in_r = color.red();
        int in_g = color.green();
        int in_b = color.blue();
        fixed out_r = in_r * lut[0] + in_g * lut[1] + in_b * lut[2];
        fixed out_g = in_r * lut[3] + in_g * lut[4] + in_b * lut[5];
        fixed out_b = in_r * lut[6] + in_g * lut[7] + in_b * lut[8];
        tonc_dst_ptr[index] = RGB15(
                    clamp(out_r.right_shift_integer(), 0, 31),
                    clamp(out_g.right_shift_integer(), 0, 31),
                    clamp(out_b.right_shift_integer(), 0, 31));
    }
}

void rotate(const color* source_colors_ptr, int rotate_count, int colors_count, color* destination_colors_ptr)
{
    int destination_index = rotate_count;

    if(destination_index < 0)
    {
        destination_index += colors_count;
    }

    for(int source_index = 0; source_index < colors_count; ++source_index)
    {
        if(destination_index >= colors_count)
        {
            destination_index -= colors_count;
        }

        destination_colors_ptr[destination_index] = source_colors_ptr[source_index];
        ++destination_index;
    }
}

}
