#include "btn_sprite_palette_item.h"

#include "btn_optional.h"
#include "btn_sprite_palette_ptr.h"

namespace btn
{

optional<sprite_palette_ptr> sprite_palette_item::find_palette() const
{
    return sprite_palette_ptr::find(*this);
}

sprite_palette_ptr sprite_palette_item::create_palette() const
{
    return sprite_palette_ptr::find_or_create(*this);
}

sprite_palette_ptr sprite_palette_item::create_new_palette() const
{
    return sprite_palette_ptr::create(*this);
}

optional<sprite_palette_ptr> sprite_palette_item::create_palette_optional() const
{
    return sprite_palette_ptr::find_or_create_optional(*this);
}

optional<sprite_palette_ptr> sprite_palette_item::create_new_palette_optional() const
{
    return sprite_palette_ptr::create_optional(*this);
}

}
