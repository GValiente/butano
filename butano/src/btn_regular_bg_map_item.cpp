#include "btn_regular_bg_map_item.h"

#include "btn_optional.h"
#include "btn_bg_tiles_ptr.h"
#include "btn_bg_palette_ptr.h"
#include "btn_regular_bg_map_ptr.h"

namespace btn
{

regular_bg_map_ptr regular_bg_map_item::create_map(bg_tiles_ptr tiles_ptr, bg_palette_ptr palette_ptr,
                                                   create_mode create_mode) const
{
    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        return regular_bg_map_ptr::find_or_create(*_cells_ptr, _dimensions, move(tiles_ptr), move(palette_ptr));

    case create_mode::FORCE_CREATE:
        return regular_bg_map_ptr::create(*_cells_ptr, _dimensions, move(tiles_ptr), move(palette_ptr));
    }

    BTN_ERROR("Invalid create mode: ", int(create_mode));
}

optional<regular_bg_map_ptr> regular_bg_map_item::optional_create_map(
        bg_tiles_ptr tiles_ptr, bg_palette_ptr palette_ptr, create_mode create_mode) const
{
    optional<regular_bg_map_ptr> result;

    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        result = regular_bg_map_ptr::optional_find_or_create(*_cells_ptr, _dimensions, move(tiles_ptr),
                                                             move(palette_ptr));
        break;

    case create_mode::FORCE_CREATE:
        result = regular_bg_map_ptr::optional_create(*_cells_ptr, _dimensions, move(tiles_ptr), move(palette_ptr));
        break;
    }

    return result;
}

}
