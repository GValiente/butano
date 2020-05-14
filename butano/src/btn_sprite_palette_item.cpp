#include "btn_sprite_palette_item.h"

#include "btn_optional.h"
#include "btn_sprite_palette_ptr.h"

namespace btn
{

optional<sprite_palette_ptr> sprite_palette_item::find_palette() const
{
    return sprite_palette_ptr::find(_palette_ref, _bpp_mode);
}

sprite_palette_ptr sprite_palette_item::create_palette() const
{
    return sprite_palette_ptr::find_or_create(_palette_ref, _bpp_mode);
}

sprite_palette_ptr sprite_palette_item::force_create_palette() const
{
    return sprite_palette_ptr::create(_palette_ref, _bpp_mode);
}

optional<sprite_palette_ptr> sprite_palette_item::optional_create_palette() const
{
    return sprite_palette_ptr::optional_find_or_create(_palette_ref, _bpp_mode);
}

optional<sprite_palette_ptr> sprite_palette_item::optional_force_create_palette() const
{
    return sprite_palette_ptr::optional_create(_palette_ref, _bpp_mode);
}

}
