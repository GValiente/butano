/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_PALETTES_H
#define BN_HW_PALETTES_H

#include "bn_color.h"
#include "bn_hw_dma.h"
#include "bn_hw_memory.h"

extern "C"
{
    [[nodiscard]] BN_CODE_IWRAM unsigned bn_hw_palettes_different_words(
            unsigned four_words_count, const unsigned* color_words, const unsigned* stored_color_words);
}

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

        inline void commit(const color* source_colors_ptr, int offset, int count, color* destination_colors_ptr,
                           bool use_dma)
        {
            const void* source = source_colors_ptr + offset;
            int words = count / 2;
            void* destination = destination_colors_ptr + offset;

            if(use_dma)
            {
                hw::dma::copy_words(source, words, destination);
            }
            else
            {
                hw::memory::copy_words(source, words, destination);
            }
        }
    }

    BN_CODE_IWRAM void brightness(
            const color* source_colors_ptr, int value, int count, color* destination_colors_ptr);

    void contrast(const color* source_colors_ptr, int value, int count, color* destination_colors_ptr);

    void intensity(const color* source_colors_ptr, int value, int count, color* destination_colors_ptr);

    inline void invert(const color* source_colors_ptr, int count, color* destination_colors_ptr)
    {
        auto tonc_src_ptr = reinterpret_cast<const COLOR*>(source_colors_ptr);
        auto tonc_dst_ptr = reinterpret_cast<COLOR*>(destination_colors_ptr);

        for(int index = 0; index < count; ++index)
        {
            tonc_dst_ptr[index] = 32767 ^ tonc_src_ptr[index];
        }
    }

    inline void aligned_invert(const color* source_colors_ptr, int count, color* destination_colors_ptr)
    {
        auto u32_src_ptr = reinterpret_cast<const uint32_t*>(source_colors_ptr);
        auto u32_dst_ptr = reinterpret_cast<uint32_t*>(destination_colors_ptr);

        for(int index = 0, limit = count / 2; index < limit; ++index)
        {
            u32_dst_ptr[index] = ((32767 << 16) + 32767) ^ u32_src_ptr[index];
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
            alignas(int) COLOR temp_colors[colors()];
            clr_grayscale(temp_colors, tonc_src_ptr, unsigned(count));
            clr_blend_fast(tonc_src_ptr, temp_colors, tonc_dst_ptr, unsigned(count), unsigned(intensity));
        }
    }

    void hue_shift(const color* source_colors_ptr, int value, int count, color* destination_colors_ptr);

    inline void blend(const color* first_source_colors_ptr, const color* second_source_colors_ptr,
                      int intensity, int count, color* destination_colors_ptr)
    {
        auto first_tonc_src_ptr = const_cast<COLOR*>(reinterpret_cast<const COLOR*>(first_source_colors_ptr));
        auto second_tonc_src_ptr = const_cast<COLOR*>(reinterpret_cast<const COLOR*>(second_source_colors_ptr));
        auto tonc_dst_ptr = reinterpret_cast<COLOR*>(destination_colors_ptr);
        clr_blend_fast(first_tonc_src_ptr, second_tonc_src_ptr, tonc_dst_ptr, unsigned(count), unsigned(intensity));
    }

    inline void fade(const color* source_colors_ptr, color fade_color, int intensity, int count,
                     color* destination_colors_ptr)
    {
        auto tonc_dst_ptr = reinterpret_cast<COLOR*>(destination_colors_ptr);
        auto tonc_fade_color = COLOR(fade_color.data());

        if(intensity == 32)
        {
            hw::memory::set_half_words(tonc_fade_color, count, tonc_dst_ptr);
        }
        else
        {
            auto const_tonc_src_ptr = reinterpret_cast<const COLOR*>(source_colors_ptr);
            auto tonc_src_ptr = const_cast<COLOR*>(const_tonc_src_ptr);
            clr_fade_fast(tonc_src_ptr, tonc_fade_color, tonc_dst_ptr, unsigned(count), unsigned(intensity));
        }
    }

    void rotate(const color* source_colors_ptr, int rotate_count, int colors_count, color* destination_colors_ptr);

    inline void commit_sprites(const color* colors_ptr, int offset, int count, bool use_dma)
    {
        commit(colors_ptr, offset, count, reinterpret_cast<color*>(MEM_PAL_OBJ), use_dma);
    }

    inline void commit_bgs(const color* colors_ptr, int offset, int count, bool use_dma)
    {
        commit(colors_ptr, offset, count, reinterpret_cast<color*>(MEM_PAL_BG), use_dma);
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

    BN_CODE_IWRAM void _lut_effect(
            const color* source_colors_ptr, const uint8_t* lut, int count, color* destination_colors_ptr);
}

#endif
