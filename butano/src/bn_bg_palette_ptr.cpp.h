/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_bg_palette_ptr.h"

#include "bn_bg_palette_item.h"
#include "bn_palettes_bank.h"
#include "bn_palettes_manager.h"
#include "../hw/include/bn_hw_palettes.h"

namespace bn
{

namespace
{
    [[nodiscard]] int _create_impl(const bg_palette_item& palette_item, bool required)
    {
        const span<const color>& colors = palette_item.colors_ref();
        palettes_bank& bg_palettes_bank = palettes_manager::bg_palettes_bank();
        int id;

        if(palette_item.bpp() == bpp_mode::BPP_4)
        {
            uint16_t hash = palettes_bank::colors_hash(colors);
            id = bg_palettes_bank.find_bpp_4(colors, hash);

            if(id < 0)
            {
                id = bg_palettes_bank.create_bpp_4(colors, hash, required);
            }
        }
        else
        {
            id = bg_palettes_bank.find_bpp_8(colors);

            if(id < 0)
            {
                id = bg_palettes_bank.create_bpp_8(colors, palette_item.compression(), required);
            }
        }

        return id;
    }

    [[nodiscard]] int _create_new_impl(const bg_palette_item& palette_item, bool required)
    {
        const span<const color>& colors = palette_item.colors_ref();
        palettes_bank& bg_palettes_bank = palettes_manager::bg_palettes_bank();
        int id;

        if(palette_item.bpp() == bpp_mode::BPP_4)
        {
            id = bg_palettes_bank.create_bpp_4(colors, palettes_bank::colors_hash(colors), required);
        }
        else
        {
            id = bg_palettes_bank.create_bpp_8(colors, palette_item.compression(), required);
        }

        return id;
    }
}

optional<bg_palette_ptr> bg_palette_ptr::find(const bg_palette_item& palette_item)
{
    optional<bg_palette_ptr> result;
    bool bpp_4 = palette_item.bpp() == bpp_mode::BPP_4;

    if(palette_item.compression() == compression_type::NONE || ! bpp_4)
    {
        const span<const color>& colors = palette_item.colors_ref();
        palettes_bank& bg_palettes_bank = palettes_manager::bg_palettes_bank();
        int id;

        if(bpp_4)
        {
            id = bg_palettes_bank.find_bpp_4(colors, palettes_bank::colors_hash(colors));
        }
        else
        {
            id = bg_palettes_bank.find_bpp_8(colors);
        }

        if(id >= 0)
        {
            result = bg_palette_ptr(id);
        }
    }
    else
    {
        alignas(int) color decompressed_colors[hw::palettes::colors()];
        result = find(palette_item.decompress(decompressed_colors));
    }

    return result;
}

bg_palette_ptr bg_palette_ptr::create(const bg_palette_item& palette_item)
{
    int id;

    if(palette_item.compression() == compression_type::NONE || palette_item.bpp() == bpp_mode::BPP_8)
    {
        id = _create_impl(palette_item, true);
    }
    else
    {
        alignas(int) color decompressed_colors[hw::palettes::colors()];
        id = _create_impl(palette_item.decompress(decompressed_colors), true);
    }

    return bg_palette_ptr(id);
}

bg_palette_ptr bg_palette_ptr::create_new(const bg_palette_item& palette_item)
{
    int id;

    if(palette_item.compression() == compression_type::NONE || palette_item.bpp() == bpp_mode::BPP_8)
    {
        id = _create_new_impl(palette_item, true);
    }
    else
    {
        alignas(int) color decompressed_colors[hw::palettes::colors()];
        id = _create_new_impl(palette_item.decompress(decompressed_colors), true);
    }

    return bg_palette_ptr(id);
}

optional<bg_palette_ptr> bg_palette_ptr::create_optional(const bg_palette_item& palette_item)
{
    int id;

    if(palette_item.compression() == compression_type::NONE || palette_item.bpp() == bpp_mode::BPP_8)
    {
        id = _create_impl(palette_item, false);
    }
    else
    {
        alignas(int) color decompressed_colors[hw::palettes::colors()];
        id = _create_impl(palette_item.decompress(decompressed_colors), false);
    }

    optional<bg_palette_ptr> result;

    if(id >= 0)
    {
        result = bg_palette_ptr(id);
    }

    return result;
}

optional<bg_palette_ptr> bg_palette_ptr::create_new_optional(const bg_palette_item& palette_item)
{
    int id;

    if(palette_item.compression() == compression_type::NONE || palette_item.bpp() == bpp_mode::BPP_8)
    {
        id = _create_new_impl(palette_item, false);
    }
    else
    {
        alignas(int) color decompressed_colors[hw::palettes::colors()];
        id = _create_new_impl(palette_item.decompress(decompressed_colors), false);
    }

    optional<bg_palette_ptr> result;

    if(id >= 0)
    {
        result = bg_palette_ptr(id);
    }

    return result;
}

bg_palette_ptr::bg_palette_ptr(const bg_palette_ptr& other) :
    _id(other._id)
{
    palettes_manager::bg_palettes_bank().increase_usages(_id);
}

bg_palette_ptr& bg_palette_ptr::operator=(const bg_palette_ptr& other)
{
    if(_id != other._id)
    {
        palettes_bank& bg_palettes_bank = palettes_manager::bg_palettes_bank();

        if(_id >= 0)
        {
            bg_palettes_bank.decrease_usages(_id);
        }

        _id = other._id;
        bg_palettes_bank.increase_usages(_id);
    }

    return *this;
}

bg_palette_ptr::~bg_palette_ptr()
{
    if(_id >= 0)
    {
        palettes_manager::bg_palettes_bank().decrease_usages(_id);
    }
}

int bg_palette_ptr::colors_count() const
{
    return palettes_manager::bg_palettes_bank().colors_count(_id);
}

span<const color> bg_palette_ptr::colors() const
{
    return palettes_manager::bg_palettes_bank().colors(_id);
}

void bg_palette_ptr::set_colors(const bg_palette_item& palette_item)
{
    palettes_bank& bg_palettes_bank = palettes_manager::bg_palettes_bank();

    if(palette_item.compression() == compression_type::NONE)
    {
        bg_palettes_bank.set_colors(_id, palette_item.colors_ref());
    }
    else
    {
        alignas(int) color decompressed_colors[hw::palettes::colors()];
        bg_palette_item decompressed_palette_item = palette_item.decompress(decompressed_colors);
        bg_palettes_bank.set_colors(_id, decompressed_palette_item.colors_ref());
    }
}

void bg_palette_ptr::set_color(int color_index, color color)
{
    palettes_manager::bg_palettes_bank().set_color(_id, color_index, color);
}

bpp_mode bg_palette_ptr::bpp() const
{
    return palettes_manager::bg_palettes_bank().bpp(_id);
}

bool bg_palette_ptr::inverted() const
{
    return palettes_manager::bg_palettes_bank().inverted(_id);
}

void bg_palette_ptr::set_inverted(bool inverted)
{
    palettes_manager::bg_palettes_bank().set_inverted(_id, inverted);
}

fixed bg_palette_ptr::grayscale_intensity() const
{
    return palettes_manager::bg_palettes_bank().grayscale_intensity(_id);
}

void bg_palette_ptr::set_grayscale_intensity(fixed intensity)
{
    palettes_manager::bg_palettes_bank().set_grayscale_intensity(_id, intensity);
}

fixed bg_palette_ptr::hue_shift_intensity() const
{
    return palettes_manager::bg_palettes_bank().hue_shift_intensity(_id);
}

void bg_palette_ptr::set_hue_shift_intensity(fixed intensity)
{
    palettes_manager::bg_palettes_bank().set_hue_shift_intensity(_id, intensity);
}

color bg_palette_ptr::fade_color() const
{
    return palettes_manager::bg_palettes_bank().fade_color(_id);
}

fixed bg_palette_ptr::fade_intensity() const
{
    return palettes_manager::bg_palettes_bank().fade_intensity(_id);
}

void bg_palette_ptr::set_fade_color(color color)
{
    palettes_manager::bg_palettes_bank().set_fade_color(_id, color);
}

void bg_palette_ptr::set_fade_intensity(fixed intensity)
{
    palettes_manager::bg_palettes_bank().set_fade_intensity(_id, intensity);
}

void bg_palette_ptr::set_fade(color color, fixed intensity)
{
    palettes_manager::bg_palettes_bank().set_fade(_id, color, intensity);
}

int bg_palette_ptr::rotate_count() const
{
    return palettes_manager::bg_palettes_bank().rotate_count(_id);
}

void bg_palette_ptr::set_rotate_count(int count)
{
    palettes_manager::bg_palettes_bank().set_rotate_count(_id, count);
}

int bg_palette_ptr::rotate_range_start() const
{
    return palettes_manager::bg_palettes_bank().rotate_range_start(_id);
}

int bg_palette_ptr::rotate_range_size() const
{
    return palettes_manager::bg_palettes_bank().rotate_range_size(_id);
}

void bg_palette_ptr::set_rotate_range(int start, int size)
{
    palettes_manager::bg_palettes_bank().set_rotate_range(_id, start, size);
}

}
