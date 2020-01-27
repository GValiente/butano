#include "btn_bg_map_item.h"

#include "btn_optional.h"
#include "btn_create_mode.h"
#include "btn_bg_map_ptr.h"

namespace btn
{

optional<bg_map_ptr> bg_map_item::create_map(create_mode create_mode) const
{
    optional<bg_map_ptr> result;

    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        result = bg_map_ptr::optional_find_or_create(*_cells_ptr, _dimensions);
        break;

    case create_mode::FORCE_CREATE:
        result = bg_map_ptr::optional_create(*_cells_ptr, _dimensions);
        break;
    }

    return result;
}

}
