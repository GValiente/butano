#include "btn_bg_tiles_item.h"

#include "btn_optional.h"
#include "btn_create_mode.h"
#include "btn_bg_tiles_ptr.h"

namespace btn
{

optional<bg_tiles_ptr> bg_tiles_item::create_tiles_ptr(create_mode create_mode) const
{
    optional<bg_tiles_ptr> result;

    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        result = bg_tiles_ptr::optional_find_or_create(_tiles);
        break;

    case create_mode::FORCE_CREATE:
        result = bg_tiles_ptr::optional_create(_tiles);
        break;
    }

    return result;
}

}
