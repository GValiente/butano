/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_PALETTES_H
#define BN_HW_PALETTES_H

#include "bn_color.h"
#include "bn_alignment.h"
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

    inline void brightness(const color* source_colors_ptr, int value, int count, color* destination_colors_ptr)
    {
        auto tonc_src_ptr = reinterpret_cast<const COLOR*>(source_colors_ptr);
        auto tonc_dst_ptr = reinterpret_cast<COLOR*>(destination_colors_ptr);
        clr_adj_brightness(tonc_dst_ptr, tonc_src_ptr, unsigned(count), value);
    }

    inline void contrast(const color* source_colors_ptr, int value, int count, color* destination_colors_ptr)
    {
        auto tonc_src_ptr = reinterpret_cast<const COLOR*>(source_colors_ptr);
        auto tonc_dst_ptr = reinterpret_cast<COLOR*>(destination_colors_ptr);
        clr_adj_contrast(tonc_dst_ptr, tonc_src_ptr, unsigned(count), value);
    }

    inline void intensity(const color* source_colors_ptr, int value, int count, color* destination_colors_ptr)
    {
        auto tonc_src_ptr = reinterpret_cast<const COLOR*>(source_colors_ptr);
        auto tonc_dst_ptr = reinterpret_cast<COLOR*>(destination_colors_ptr);
        clr_adj_intensity(tonc_dst_ptr, tonc_src_ptr, unsigned(count), value);
    }

    inline void invert(const color* source_colors_ptr, int count, color* destination_colors_ptr)
    {
        if(count % 2 == 0 && aligned<sizeof(uint32_t)>(source_colors_ptr) &&
                aligned<sizeof(uint32_t)>(destination_colors_ptr))
        {
            auto u32_src_ptr = reinterpret_cast<const uint32_t*>(source_colors_ptr);
            auto u32_dst_ptr = reinterpret_cast<uint32_t*>(destination_colors_ptr);

            for(int index = 0, limit = count / 2; index < limit; ++index)
            {
                u32_dst_ptr[index] = ((32767 << 16) + 32767) ^ u32_src_ptr[index];
            }
        }
        else
        {
            auto tonc_src_ptr = reinterpret_cast<const COLOR*>(source_colors_ptr);
            auto tonc_dst_ptr = reinterpret_cast<COLOR*>(destination_colors_ptr);

            for(int index = 0; index < count; ++index)
            {
                tonc_dst_ptr[index] = 32767 ^ tonc_src_ptr[index];
            }
        }
    }

    inline void grayscale(const color* source_colors_ptr, int intensity, int count, color* destination_colors_ptr)
    {
        auto const_tonc_src_ptr = reinterpret_cast<const COLOR*>(source_colors_ptr);
        auto tonc_src_ptr = const_cast<COLOR*>(const_tonc_src_ptr);
        auto tonc_dst_ptr = reinterpret_cast<COLOR*>(destination_colors_ptr);

        if(intensity == 32)
        {
            clr_grayscale(tonc_dst_ptr, tonc_src_ptr, unsigned(count));
        }
        else
        {
            COLOR temp_colors[colors()];
            clr_grayscale(temp_colors, tonc_src_ptr, unsigned(count));
            clr_blend_fast(tonc_src_ptr, temp_colors, tonc_dst_ptr, unsigned(count), unsigned(intensity));
        }
    }

    inline void fade(const color* source_colors_ptr, color fade_color, int intensity, int count,
                     color* destination_colors_ptr)
    {
        auto const_tonc_src_ptr = reinterpret_cast<const COLOR*>(source_colors_ptr);
        auto tonc_src_ptr = const_cast<COLOR*>(const_tonc_src_ptr);
        auto tonc_dst_ptr = reinterpret_cast<COLOR*>(destination_colors_ptr);
        clr_fade_fast(tonc_src_ptr, uint16_t(fade_color.data()), tonc_dst_ptr, unsigned(count), unsigned(intensity));
    }

    inline void rotate(const color* source_colors_ptr, int rotate_count, int colors_count,
                       color* destination_colors_ptr)
    {
        auto const_tonc_src_ptr = reinterpret_cast<const COLOR*>(source_colors_ptr);
        auto tonc_src_ptr = const_cast<COLOR*>(const_tonc_src_ptr);
        auto tonc_dst_ptr = reinterpret_cast<COLOR*>(destination_colors_ptr);

        if(tonc_src_ptr != tonc_dst_ptr)
        {
            memory::copy_half_words(tonc_src_ptr, colors_count, tonc_dst_ptr);
        }

        clr_rotate(tonc_dst_ptr, unsigned(colors_count), rotate_count);
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
