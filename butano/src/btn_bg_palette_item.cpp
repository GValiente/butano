#include "btn_bg_palette_item.h"

#include "btn_optional.h"
#include "btn_bg_palette_ptr.h"

namespace btn
{

bg_palette_ptr bg_palette_item::create_palette(create_mode create_mode) const
{
    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        return bg_palette_ptr::find_or_create(_palette_ref, _bpp_mode);

    case create_mode::FORCE_CREATE:
        return bg_palette_ptr::create(_palette_ref, _bpp_mode);
    }

    BTN_ERROR("Invalid create mode: ", int(create_mode));
}

optional<bg_palette_ptr> bg_palette_item::optional_create_palette(create_mode create_mode) const
{
    optional<bg_palette_ptr> result;

    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        result = bg_palette_ptr::optional_find_or_create(_palette_ref, _bpp_mode);
        break;

    case create_mode::FORCE_CREATE:
        result = bg_palette_ptr::optional_create(_palette_ref, _bpp_mode);
        break;
    }

    return result;
}

}
