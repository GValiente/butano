#include "btn_regular_bg_map_item.h"

#include "btn_optional.h"
#include "btn_create_mode.h"
#include "btn_regular_bg_map_ptr.h"

namespace btn
{

optional<regular_bg_map_ptr> regular_bg_map_item::create_map(const bg_palette_ptr& palette_ptr,
                                                             create_mode create_mode) const
{
    optional<regular_bg_map_ptr> result;

    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        result = regular_bg_map_ptr::optional_find_or_create(*_cells_ptr, _dimensions, palette_ptr);
        break;

    case create_mode::FORCE_CREATE:
        result = regular_bg_map_ptr::optional_create(*_cells_ptr, _dimensions, palette_ptr);
        break;
    }

    return result;
}

optional<regular_bg_map_ptr> regular_bg_map_item::create_map(bg_palette_ptr&& palette_ptr,
                                                             create_mode create_mode) const
{
    optional<regular_bg_map_ptr> result;

    switch(create_mode)
    {

    case create_mode::FIND_OR_CREATE:
        result = regular_bg_map_ptr::optional_find_or_create(*_cells_ptr, _dimensions, move(palette_ptr));
        break;

    case create_mode::FORCE_CREATE:
        result = regular_bg_map_ptr::optional_create(*_cells_ptr, _dimensions, move(palette_ptr));
        break;
    }

    return result;
}

}
