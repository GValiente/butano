#ifndef BTN_REGULAR_BG_ITEM_H
#define BTN_REGULAR_BG_ITEM_H

#include "btn_bg_tiles_item.h"
#include "btn_bg_palette_item.h"
#include "btn_regular_bg_map_item.h"

namespace btn
{

class fixed_point;
class regular_bg_ptr;

class regular_bg_item
{

public:
    constexpr regular_bg_item(const span<const tile>& tiles, const regular_bg_map_cell& map_cells_ref,
                              const size& map_dimensions, const span<const color>& palette,
                              palette_bpp_mode bpp_mode) :
        regular_bg_item(bg_tiles_item(tiles), regular_bg_map_item(map_cells_ref, map_dimensions),
                        bg_palette_item(palette, bpp_mode))
    {
    }

    constexpr regular_bg_item(const bg_tiles_item& tiles_item, const regular_bg_map_item& map_item,
                              const bg_palette_item& palette_item) :
        _tiles_item(tiles_item),
        _map_item(map_item),
        _palette_item(palette_item)
    {
        BTN_CONSTEXPR_ASSERT(tiles_item.valid_tiles_count(palette_item.bpp_mode()), "Invalid tiles count");
    }

    [[nodiscard]] constexpr const bg_tiles_item& tiles_item() const
    {
        return _tiles_item;
    }

    [[nodiscard]] constexpr const regular_bg_map_item& map_item() const
    {
        return _map_item;
    }

    [[nodiscard]] constexpr const bg_palette_item& palette_item() const
    {
        return _palette_item;
    }

    [[nodiscard]] regular_bg_ptr create_bg(fixed x, fixed y) const;

    [[nodiscard]] regular_bg_ptr create_bg(const fixed_point& position) const;

    [[nodiscard]] optional<regular_bg_ptr> optional_create_bg(fixed x, fixed y) const;

    [[nodiscard]] optional<regular_bg_ptr> optional_create_bg(const fixed_point& position) const;

    [[nodiscard]] constexpr friend bool operator==(const regular_bg_item& a, const regular_bg_item& b)
    {
        return a._tiles_item == b._tiles_item && a._map_item == b._map_item && a._palette_item == b._palette_item;
    }

    [[nodiscard]] constexpr friend bool operator!=(const regular_bg_item& a, const regular_bg_item& b)
    {
        return ! (a == b);
    }

private:
    bg_tiles_item _tiles_item;
    regular_bg_map_item _map_item;
    bg_palette_item _palette_item;
};

}

#endif

