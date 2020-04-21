#include "btn_sprite_tiles_item.h"

#include "btn_optional.h"
#include "btn_sprite_tiles_ptr.h"

namespace btn
{

sprite_tiles_ptr sprite_tiles_item::create_tiles() const
{
    return sprite_tiles_ptr::find_or_create(graphics_tiles_ref());
}

sprite_tiles_ptr sprite_tiles_item::force_create_tiles() const
{
    return sprite_tiles_ptr::create(graphics_tiles_ref());
}

sprite_tiles_ptr sprite_tiles_item::create_tiles(int graphics_index) const
{
    return sprite_tiles_ptr::find_or_create(graphics_tiles_ref(graphics_index));
}

sprite_tiles_ptr sprite_tiles_item::force_create_tiles(int graphics_index) const
{
    return sprite_tiles_ptr::create(graphics_tiles_ref(graphics_index));
}

optional<sprite_tiles_ptr> sprite_tiles_item::optional_create_tiles() const
{
    return sprite_tiles_ptr::optional_find_or_create(graphics_tiles_ref());
}

optional<sprite_tiles_ptr> sprite_tiles_item::optional_force_create_tiles() const
{
    return sprite_tiles_ptr::optional_create(graphics_tiles_ref());
}

optional<sprite_tiles_ptr> sprite_tiles_item::optional_create_tiles(int graphics_index) const
{
    return sprite_tiles_ptr::optional_find_or_create(graphics_tiles_ref(graphics_index));
}

optional<sprite_tiles_ptr> sprite_tiles_item::optional_force_create_tiles(int graphics_index) const
{
    return sprite_tiles_ptr::optional_create(graphics_tiles_ref(graphics_index));
}

}
