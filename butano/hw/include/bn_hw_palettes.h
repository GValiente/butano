/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_PALETTES_H
#define BN_HW_PALETTES_H

#include "bn_color.h"
#include "bn_hw_tonc.h"
#include "bn_hw_memory.h"

namespace bn::hw::palettes
{
    [[nodiscard]] constexpr int count()
    {
        return 16;
    }

    [[nodiscard]] constexpr int colors_per_palette()
    {
        return 16;
    }

    [[nodiscard]] constexpr int colors()
    {
        return count() * colors_per_palette();
    }

    namespace
    {
        static_assert(sizeof(color) == sizeof(COLOR));
        static_assert(alignof(color) == alignof(COLOR));

        inline void commit(const color* source_colors_ptr, int offset, int count, color* destination_colors_ptr)
        {
            hw::memory::copy_words(source_colors_ptr + offset, count / 2, destination_colors_ptr + offset);
        }
    }

    inline void brightness(int value, int count, color* colors_ptr)
    {
        auto tonc_colors_ptr = reinterpret_cast<COLOR*>(colors_ptr);
        clr_adj_brightness(tonc_colors_ptr, tonc_colors_ptr, unsigned(count), value);
    }

    inline void contrast(int value, int count, color* colors_ptr)
    {
        auto tonc_colors_ptr = reinterpret_cast<COLOR*>(colors_ptr);
        clr_adj_contrast(tonc_colors_ptr, tonc_colors_ptr, unsigned(count), value);
    }

    inline void intensity(int value, int count, color* colors_ptr)
    {
        auto tonc_colors_ptr = reinterpret_cast<COLOR*>(colors_ptr);
        clr_adj_intensity(tonc_colors_ptr, tonc_colors_ptr, unsigned(count), value);
    }

    inline void invert(int count, color* colors_ptr)
    {
        auto tonc_colors_ptr = reinterpret_cast<COLOR*>(colors_ptr);

        for(int index = 0; index < count; ++index)
        {
            tonc_colors_ptr[index] = 32767 ^ tonc_colors_ptr[index];
        }
    }

    inline void grayscale(int intensity, int count, color* colors_ptr)
    {
        auto tonc_colors_ptr = reinterpret_cast<COLOR*>(colors_ptr);
        COLOR temp_colors[colors()];
        clr_grayscale(temp_colors, tonc_colors_ptr, unsigned(count));
        clr_blend_fast(tonc_colors_ptr, temp_colors, tonc_colors_ptr, unsigned(count), unsigned(intensity));
    }

    inline void fade(color fade_color, int intensity, int count, color* colors_ptr)
    {
        auto tonc_colors_ptr = reinterpret_cast<COLOR*>(colors_ptr);
        clr_fade_fast(tonc_colors_ptr, uint16_t(fade_color.data()), tonc_colors_ptr, unsigned(count), unsigned(intensity));
    }

    inline void rotate(int rotate_count, int colors_count, color* colors_ptr)
    {
        auto tonc_colors_ptr = reinterpret_cast<COLOR*>(colors_ptr);
        clr_rotate(tonc_colors_ptr, unsigned(colors_count), rotate_count);
    }

    inline void commit_sprites(const color* colors_ptr, int offset, int count)
    {
        commit(colors_ptr, offset, count, reinterpret_cast<color*>(MEM_PAL_OBJ));
    }

    inline void commit_bgs(const color* colors_ptr, int offset, int count)
    {
        commit(colors_ptr, offset, count, reinterpret_cast<color*>(MEM_PAL_BG));
    }

    [[nodiscard]] inline uint16_t* sprite_color_register(int index)
    {
        return reinterpret_cast<uint16_t*>(MEM_PAL_OBJ) + index;
    }

    [[nodiscard]] inline uint16_t* bg_color_register(int index)
    {
        return reinterpret_cast<uint16_t*>(MEM_PAL_BG) + index;
    }

    [[nodiscard]] inline uint16_t* bg_transparent_color_register()
    {
        return reinterpret_cast<uint16_t*>(MEM_PAL_BG);
    }
}

#endif
