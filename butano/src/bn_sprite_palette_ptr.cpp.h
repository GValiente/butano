/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_sprite_palette_ptr.h"

#include "bn_sprite_palette_item.h"
#include "bn_palettes_bank.h"
#include "bn_palettes_manager.h"

namespace bn
{

optional<sprite_palette_ptr> sprite_palette_ptr::find(const sprite_palette_item& palette_item)
{
    const span<const color>& colors = palette_item.colors_ref();
    palettes_bank& sprite_palettes_bank = palettes_manager::sprite_palettes_bank();
    int id;

    if(palette_item.bpp() == bpp_mode::BPP_4)
    {
        id = sprite_palettes_bank.find_bpp_4(colors, palettes_bank::colors_hash(colors));
    }
    else
    {
        id = sprite_palettes_bank.find_bpp_8(colors);
    }

    optional<sprite_palette_ptr> result;

    if(id >= 0)
    {
        result = sprite_palette_ptr(id);
    }

    return result;
}

sprite_palette_ptr sprite_palette_ptr::create(const sprite_palette_item& palette_item)
{
    const span<const color>& colors = palette_item.colors_ref();
    palettes_bank& sprite_palettes_bank = palettes_manager::sprite_palettes_bank();
    int id;

    if(palette_item.bpp() == bpp_mode::BPP_4)
    {
        uint16_t hash = palettes_bank::colors_hash(colors);
        id = sprite_palettes_bank.find_bpp_4(colors, hash);

        if(id < 0)
        {
            id = sprite_palettes_bank.create_bpp_4(colors, hash, true);
        }
    }
    else
    {
        id = sprite_palettes_bank.find_bpp_8(colors);

        if(id < 0)
        {
            id = sprite_palettes_bank.create_bpp_8(colors, true);
        }
    }

    return sprite_palette_ptr(id);
}

sprite_palette_ptr sprite_palette_ptr::create_new(const sprite_palette_item& palette_item)
{
    const span<const color>& colors = palette_item.colors_ref();
    palettes_bank& sprite_palettes_bank = palettes_manager::sprite_palettes_bank();
    int id;

    if(palette_item.bpp() == bpp_mode::BPP_4)
    {
        id = sprite_palettes_bank.create_bpp_4(colors, palettes_bank::colors_hash(colors), true);
    }
    else
    {
        id = sprite_palettes_bank.create_bpp_8(colors, true);
    }

    return sprite_palette_ptr(id);
}

optional<sprite_palette_ptr> sprite_palette_ptr::create_optional(const sprite_palette_item& palette_item)
{
    const span<const color>& colors = palette_item.colors_ref();
    palettes_bank& sprite_palettes_bank = palettes_manager::sprite_palettes_bank();
    int id;

    if(palette_item.bpp() == bpp_mode::BPP_4)
    {
        uint16_t hash = palettes_bank::colors_hash(colors);
        id = sprite_palettes_bank.find_bpp_4(colors, hash);

        if(id < 0)
        {
            id = sprite_palettes_bank.create_bpp_4(colors, hash, false);
        }
    }
    else
    {
        id = sprite_palettes_bank.find_bpp_8(colors);

        if(id < 0)
        {
            id = sprite_palettes_bank.create_bpp_8(colors, false);
        }
    }

    optional<sprite_palette_ptr> result;

    if(id >= 0)
    {
        result = sprite_palette_ptr(id);
    }

    return result;
}

optional<sprite_palette_ptr> sprite_palette_ptr::create_new_optional(const sprite_palette_item& palette_item)
{
    const span<const color>& colors = palette_item.colors_ref();
    palettes_bank& sprite_palettes_bank = palettes_manager::sprite_palettes_bank();
    int id;

    if(palette_item.bpp() == bpp_mode::BPP_4)
    {
        id = sprite_palettes_bank.create_bpp_4(colors, palettes_bank::colors_hash(colors), false);
    }
    else
    {
        id = sprite_palettes_bank.create_bpp_8(colors, false);
    }

    optional<sprite_palette_ptr> result;

    if(id >= 0)
    {
        result = sprite_palette_ptr(id);
    }

    return result;
}

sprite_palette_ptr::sprite_palette_ptr(const sprite_palette_ptr& other) :
    _id(other._id)
{
    palettes_manager::sprite_palettes_bank().increase_usages(_id);
}

sprite_palette_ptr& sprite_palette_ptr::operator=(const sprite_palette_ptr& other)
{
    if(_id != other._id)
    {
        if(_id >= 0)
        {
            palettes_manager::sprite_palettes_bank().decrease_usages(_id);
        }

        _id = other._id;
        palettes_manager::sprite_palettes_bank().increase_usages(_id);
    }

    return *this;
}

int sprite_palette_ptr::colors_count() const
{
    return palettes_manager::sprite_palettes_bank().colors_count(_id);
}

span<const color> sprite_palette_ptr::colors() const
{
    return palettes_manager::sprite_palettes_bank().colors(_id);
}

void sprite_palette_ptr::set_colors(const sprite_palette_item& palette_item)
{
    palettes_manager::sprite_palettes_bank().set_colors(_id, palette_item.colors_ref());
}

bpp_mode sprite_palette_ptr::bpp() const
{
    return palettes_manager::sprite_palettes_bank().bpp(_id);
}

bool sprite_palette_ptr::inverted() const
{
    return palettes_manager::sprite_palettes_bank().inverted(_id);
}

void sprite_palette_ptr::set_inverted(bool inverted)
{
    palettes_manager::sprite_palettes_bank().set_inverted(_id, inverted);
}

fixed sprite_palette_ptr::grayscale_intensity() const
{
    return palettes_manager::sprite_palettes_bank().grayscale_intensity(_id);
}

void sprite_palette_ptr::set_grayscale_intensity(fixed intensity)
{
    palettes_manager::sprite_palettes_bank().set_grayscale_intensity(_id, intensity);
}

color sprite_palette_ptr::fade_color() const
{
    return palettes_manager::sprite_palettes_bank().fade_color(_id);
}

fixed sprite_palette_ptr::fade_intensity() const
{
    return palettes_manager::sprite_palettes_bank().fade_intensity(_id);
}

void sprite_palette_ptr::set_fade_color(color color)
{
    palettes_manager::sprite_palettes_bank().set_fade_color(_id, color);
}

void sprite_palette_ptr::set_fade_intensity(fixed intensity)
{
    palettes_manager::sprite_palettes_bank().set_fade_intensity(_id, intensity);
}

void sprite_palette_ptr::set_fade(color color, fixed intensity)
{
    palettes_manager::sprite_palettes_bank().set_fade(_id, color, intensity);
}

int sprite_palette_ptr::rotate_count() const
{
    return palettes_manager::sprite_palettes_bank().rotate_count(_id);
}

void sprite_palette_ptr::set_rotate_count(int count)
{
    palettes_manager::sprite_palettes_bank().set_rotate_count(_id, count);
}

void sprite_palette_ptr::_destroy()
{
    palettes_manager::sprite_palettes_bank().decrease_usages(_id);
}

}
