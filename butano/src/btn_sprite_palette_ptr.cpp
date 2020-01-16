#include "btn_sprite_palette_ptr.h"

#include "btn_span.h"
#include "btn_utility.h"
#include "btn_palettes_bank.h"
#include "btn_palettes_manager.h"
#include "../hw/include/btn_hw_palettes.h"

namespace btn
{

optional<sprite_palette_ptr> sprite_palette_ptr::find(const span<const color>& colors_ref)
{
    optional<sprite_palette_ptr> result;

    if(optional<int> id = palettes_manager::sprite_palettes_bank().find(colors_ref))
    {
        result = sprite_palette_ptr(*id);
    }

    return result;
}

sprite_palette_ptr sprite_palette_ptr::create(const span<const color>& colors_ref)
{
    optional<int> id = palettes_manager::sprite_palettes_bank().create(colors_ref);
    BTN_ASSERT(id, "Sprite palette create failed");

    return sprite_palette_ptr(*id);
}

optional<sprite_palette_ptr> sprite_palette_ptr::optional_create(const span<const color>& colors_ref)
{
    optional<sprite_palette_ptr> result;

    if(optional<int> id = palettes_manager::sprite_palettes_bank().create(colors_ref))
    {
        result = sprite_palette_ptr(*id);
    }

    return result;
}

sprite_palette_ptr sprite_palette_ptr::find_or_create(const span<const color>& colors_ref)
{
    palettes_bank& sprite_palettes_bank = palettes_manager::sprite_palettes_bank();
    optional<int> id = sprite_palettes_bank.find(colors_ref);

    if(! id)
    {
        id = sprite_palettes_bank.create(colors_ref);
        BTN_ASSERT(id, "Sprite palette find or create failed");
    }

    return sprite_palette_ptr(*id);
}

optional<sprite_palette_ptr> sprite_palette_ptr::optional_find_or_create(const span<const color>& colors_ref)
{
    palettes_bank& sprite_palettes_bank = palettes_manager::sprite_palettes_bank();
    optional<sprite_palette_ptr> result;

    if(optional<int> id = sprite_palettes_bank.find(colors_ref))
    {
        result = sprite_palette_ptr(*id);
    }
    else if(optional<int> id = sprite_palettes_bank.create(colors_ref))
    {
        result = sprite_palette_ptr(*id);
    }

    return result;
}

sprite_palette_ptr::sprite_palette_ptr(const sprite_palette_ptr& other) :
    palette_ptr(other._id)
{
    palettes_manager::sprite_palettes_bank().increase_usages(_id);
}

sprite_palette_ptr& sprite_palette_ptr::operator=(const sprite_palette_ptr& other)
{
    if(_id != other._id)
    {
        _destroy();
        _id = other._id;
        palettes_manager::sprite_palettes_bank().increase_usages(_id);
    }

    return *this;
}

sprite_palette_ptr::sprite_palette_ptr(sprite_palette_ptr&& other) :
    palette_ptr(other._id)
{
    other._id = -1;
}

sprite_palette_ptr& sprite_palette_ptr::operator=(sprite_palette_ptr&& other)
{
    swap(_id, other._id);
    return *this;
}

span<const color> sprite_palette_ptr::colors_ref() const
{
    return palettes_manager::sprite_palettes_bank().colors_ref(_id);
}

void sprite_palette_ptr::set_colors_ref(const span<const color>& colors_ref)
{
    palettes_manager::sprite_palettes_bank().set_colors_ref(_id, colors_ref);
}

void sprite_palette_ptr::reload_colors_ref()
{
    palettes_manager::sprite_palettes_bank().reload_colors_ref(_id);
}

int sprite_palette_ptr::colors_count() const
{
    return int(colors_ref().size());
}

bool sprite_palette_ptr::eight_bits_per_pixel() const
{
    return colors_ref().size() > hw::palettes::colors_per_palette();
}

fixed sprite_palette_ptr::inverse_intensity() const
{
    return palettes_manager::sprite_palettes_bank().inverse_intensity(_id);
}

void sprite_palette_ptr::set_inverse_intensity(fixed intensity)
{
    palettes_manager::sprite_palettes_bank().set_inverse_intensity(_id, intensity);
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
    if(_id >= 0)
    {
        palettes_manager::sprite_palettes_bank().decrease_usages(_id);
        _id = -1;
    }
}

}
