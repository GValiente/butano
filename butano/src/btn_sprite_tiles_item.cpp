#include "btn_sprite_tiles_item.h"

#include "btn_create_mode.h"
#include "btn_sprite_tiles_ptr.h"

namespace btn
{

sprite_tiles_ptr sprite_tiles_item::tiles_ptr(int graphics_id, create_mode create_mode) const
{
    span<const tile> graphics_tiles_ref = tiles_ref(graphics_id);

    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        return sprite_tiles_ptr::find_or_create(graphics_tiles_ref);

    case create_mode::FORCE_CREATE:
        return sprite_tiles_ptr::create(graphics_tiles_ref);
    }

    BTN_ERROR("Invalid create mode: ", int(create_mode));
    return sprite_tiles_ptr::create(graphics_tiles_ref);
}

}
