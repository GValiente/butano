#include "btn_bg_tiles_item.h"

#include "btn_optional.h"
#include "btn_bg_tiles_ptr.h"

namespace btn
{

bg_tiles_ptr bg_tiles_item::create_tiles(create_mode create_mode) const
{
    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        return bg_tiles_ptr::find_or_create(_tiles_ref);

    case create_mode::FORCE_CREATE:
        return bg_tiles_ptr::create(_tiles_ref);
    }

    BTN_ERROR("Invalid create mode: ", int(create_mode));
}

optional<bg_tiles_ptr> bg_tiles_item::optional_create_tiles(create_mode create_mode) const
{
    optional<bg_tiles_ptr> result;

    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        result = bg_tiles_ptr::optional_find_or_create(_tiles_ref);
        break;

    case create_mode::FORCE_CREATE:
        result = bg_tiles_ptr::optional_create(_tiles_ref);
        break;
    }

    return result;
}

}
