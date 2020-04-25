#include "btn_sprite_item.h"

#include "btn_fixed.h"
#include "btn_optional.h"
#include "btn_sprite_ptr.h"

namespace btn
{

sprite_ptr sprite_item::create_sprite(fixed x, fixed y) const
{
    return sprite_ptr::create(x, y, *this);
}

sprite_ptr sprite_item::create_sprite(fixed x, fixed y, int graphics_index) const
{
    return sprite_ptr::create(x, y, *this, graphics_index);
}

sprite_ptr sprite_item::create_sprite(const fixed_point& position) const
{
    return sprite_ptr::create(position, *this);
}

sprite_ptr sprite_item::create_sprite(const fixed_point& position, int graphics_index) const
{
    return sprite_ptr::create(position, *this, graphics_index);
}

optional<sprite_ptr> sprite_item::optional_create_sprite(fixed x, fixed y) const
{
    return sprite_ptr::optional_create(x, y, *this);
}

optional<sprite_ptr> sprite_item::optional_create_sprite(fixed x, fixed y, int graphics_index) const
{
    return sprite_ptr::optional_create(x, y, *this, graphics_index);
}

optional<sprite_ptr> sprite_item::optional_create_sprite(const fixed_point& position) const
{
    return sprite_ptr::optional_create(position, *this);
}

optional<sprite_ptr> sprite_item::optional_create_sprite(const fixed_point& position, int graphics_index) const
{
    return sprite_ptr::optional_create(position, *this, graphics_index);
}

}
