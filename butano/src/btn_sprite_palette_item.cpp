#include "btn_sprite_palette_item.h"

#include "btn_optional.h"
#include "btn_create_mode.h"
#include "btn_sprite_palette_ptr.h"

namespace btn
{

optional<sprite_palette_ptr> sprite_palette_item::palette_ptr(create_mode create_mode) const
{
    optional<sprite_palette_ptr> result;

    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        result = sprite_palette_ptr::optional_find_or_create(_palette);
        break;

    case create_mode::FORCE_CREATE:
        result = sprite_palette_ptr::optional_create(_palette);
        break;
    }

    return result;
}

}
