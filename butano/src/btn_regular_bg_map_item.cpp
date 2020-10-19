#include "btn_regular_bg_map_item.h"

#include "btn_optional.h"
#include "btn_bg_tiles_ptr.h"
#include "btn_bg_palette_ptr.h"
#include "btn_regular_bg_map_ptr.h"

namespace btn
{

optional<regular_bg_map_ptr> regular_bg_map_item::find_map(
        const bg_tiles_ptr& tiles, const bg_palette_ptr& palette) const
{
    return regular_bg_map_ptr::find(*this, tiles, palette);
}

regular_bg_map_ptr regular_bg_map_item::create_map(bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return regular_bg_map_ptr::find_or_create(*this, move(tiles), move(palette));
}

regular_bg_map_ptr regular_bg_map_item::create_new_map(bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return regular_bg_map_ptr::create(*this, move(tiles), move(palette));
}

optional<regular_bg_map_ptr> regular_bg_map_item::create_map_optional(bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return regular_bg_map_ptr::find_or_create_optional(*this, move(tiles), move(palette));
}

optional<regular_bg_map_ptr> regular_bg_map_item::create_new_map_optional(
        bg_tiles_ptr tiles, bg_palette_ptr palette) const
{
    return regular_bg_map_ptr::create_optional(*this, move(tiles), move(palette));
}

}
