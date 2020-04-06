#include "btn_sprite_tiles_item.h"

#include "btn_optional.h"
#include "btn_sprite_tiles_ptr.h"

namespace btn
{

sprite_tiles_ptr sprite_tiles_item::create_tiles(int graphics_index, create_mode create_mode) const
{
    span<const tile> graphics_tiles_ref = tiles_ref(graphics_index);

    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        return sprite_tiles_ptr::find_or_create(graphics_tiles_ref);

    case create_mode::FORCE_CREATE:
        return sprite_tiles_ptr::create(graphics_tiles_ref);
    }

    BTN_ERROR("Invalid create mode: ", int(create_mode));
}

optional<sprite_tiles_ptr> sprite_tiles_item::optional_create_tiles(int graphics_index, create_mode create_mode) const
{
    span<const tile> graphics_tiles_ref = tiles_ref(graphics_index);
    optional<sprite_tiles_ptr> result;

    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        result = sprite_tiles_ptr::optional_find_or_create(graphics_tiles_ref);
        break;

    case create_mode::FORCE_CREATE:
        result = sprite_tiles_ptr::optional_create(graphics_tiles_ref);
        break;
    }

    return result;
}

}
