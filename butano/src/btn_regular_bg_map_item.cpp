#include "btn_regular_bg_map_item.h"

#include "btn_optional.h"
#include "btn_bg_tiles_ptr.h"
#include "btn_bg_palette_ptr.h"
#include "btn_regular_bg_map_ptr.h"

namespace btn
{

regular_bg_map_ptr regular_bg_map_item::create_map(bg_tiles_ptr tiles_ptr, bg_palette_ptr palette_ptr) const
{
    return regular_bg_map_ptr::find_or_create(*_cells_ptr, _dimensions, move(tiles_ptr), move(palette_ptr));
}

regular_bg_map_ptr regular_bg_map_item::force_create_map(bg_tiles_ptr tiles_ptr, bg_palette_ptr palette_ptr) const
{
    return regular_bg_map_ptr::create(*_cells_ptr, _dimensions, move(tiles_ptr), move(palette_ptr));
}

optional<regular_bg_map_ptr> regular_bg_map_item::optional_create_map(
        bg_tiles_ptr tiles_ptr, bg_palette_ptr palette_ptr) const
{
    return regular_bg_map_ptr::optional_find_or_create(*_cells_ptr, _dimensions, move(tiles_ptr), move(palette_ptr));
}

optional<regular_bg_map_ptr> regular_bg_map_item::optional_force_create_map(
        bg_tiles_ptr tiles_ptr, bg_palette_ptr palette_ptr) const
{
    return regular_bg_map_ptr::optional_create(*_cells_ptr, _dimensions, move(tiles_ptr), move(palette_ptr));
}

}
