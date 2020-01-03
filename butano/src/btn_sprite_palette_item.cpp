#include "btn_sprite_palette_item.h"

#include "btn_color.h"
#include "btn_create_mode.h"
#include "btn_sprite_palette_ptr.h"

namespace btn
{

sprite_palette_ptr sprite_palette_item::palette_ptr(create_mode create_mode) const
{
    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        return sprite_palette_ptr::find_or_create(_palette);

    case create_mode::FORCE_CREATE:
        return sprite_palette_ptr::create(_palette);
    }

    BTN_ERROR("Invalid create mode: ", int(create_mode));
    return sprite_palette_ptr::create(_palette);
}

}
