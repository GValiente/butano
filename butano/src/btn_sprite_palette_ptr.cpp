#include "btn_sprite_palette_ptr.h"

#include "btn_palettes_bank.h"
#include "btn_palette_bpp_mode.h"
#include "btn_palettes_manager.h"

namespace btn
{

optional<sprite_palette_ptr> sprite_palette_ptr::find(const span<const color>& colors, palette_bpp_mode bpp_mode)
{
    palettes_bank& sprite_palettes_bank = palettes_manager::sprite_palettes_bank();
    int id;

    if(bpp_mode == palette_bpp_mode::BPP_4)
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

sprite_palette_ptr sprite_palette_ptr::create(const span<const color>& colors, palette_bpp_mode bpp_mode)
{
    palettes_bank& sprite_palettes_bank = palettes_manager::sprite_palettes_bank();
    int id;

    if(bpp_mode == palette_bpp_mode::BPP_4)
    {
        id = sprite_palettes_bank.create_bpp_4(colors, palettes_bank::colors_hash(colors));
    }
    else
    {
        id = sprite_palettes_bank.create_bpp_8(colors);
    }

    BTN_ASSERT(id >= 0, "Palette create failed");

    return sprite_palette_ptr(id);
}

sprite_palette_ptr sprite_palette_ptr::find_or_create(const span<const color>& colors, palette_bpp_mode bpp_mode)
{
    palettes_bank& sprite_palettes_bank = palettes_manager::sprite_palettes_bank();
    int id;

    if(bpp_mode == palette_bpp_mode::BPP_4)
    {
        unsigned hash = palettes_bank::colors_hash(colors);
        id = sprite_palettes_bank.find_bpp_4(colors, hash);

        if(id < 0)
        {
            id = sprite_palettes_bank.create_bpp_4(colors, hash);
            BTN_ASSERT(id >= 0, "Palette find or create failed");
        }
    }
    else
    {
        id = sprite_palettes_bank.find_bpp_8(colors);

        if(id < 0)
        {
            id = sprite_palettes_bank.create_bpp_8(colors);
            BTN_ASSERT(id >= 0, "Palette find or create failed");
        }
    }

    return sprite_palette_ptr(id);
}

optional<sprite_palette_ptr> sprite_palette_ptr::create_optional(const span<const color>& colors,
                                                                 palette_bpp_mode bpp_mode)
{
    palettes_bank& sprite_palettes_bank = palettes_manager::sprite_palettes_bank();
    int id;

    if(bpp_mode == palette_bpp_mode::BPP_4)
    {
        id = sprite_palettes_bank.create_bpp_4(colors, palettes_bank::colors_hash(colors));
    }
    else
    {
        id = sprite_palettes_bank.create_bpp_8(colors);
    }

    optional<sprite_palette_ptr> result;

    if(id >= 0)
    {
        result = sprite_palette_ptr(id);
    }

    return result;
}

optional<sprite_palette_ptr> sprite_palette_ptr::find_or_create_optional(const span<const color>& colors,
                                                                         palette_bpp_mode bpp_mode)
{
    palettes_bank& sprite_palettes_bank = palettes_manager::sprite_palettes_bank();
    int id;

    if(bpp_mode == palette_bpp_mode::BPP_4)
    {
        unsigned hash = palettes_bank::colors_hash(colors);
        id = sprite_palettes_bank.find_bpp_4(colors, hash);

        if(id < 0)
        {
            id = sprite_palettes_bank.create_bpp_4(colors, hash);
        }
    }
    else
    {
        id = sprite_palettes_bank.find_bpp_8(colors);

        if(id < 0)
        {
            id = sprite_palettes_bank.create_bpp_8(colors);
        }
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

span<const color> sprite_palette_ptr::colors() const
{
    return palettes_manager::sprite_palettes_bank().colors(_id);
}

void sprite_palette_ptr::set_colors(const span<const color>& colors)
{
    palettes_manager::sprite_palettes_bank().set_colors(_id, colors);
}

int sprite_palette_ptr::colors_count() const
{
    return palettes_manager::sprite_palettes_bank().colors_count(_id);
}

palette_bpp_mode sprite_palette_ptr::bpp_mode() const
{
    return palettes_manager::sprite_palettes_bank().bpp_mode(_id);
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
