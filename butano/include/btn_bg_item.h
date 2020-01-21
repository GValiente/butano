#ifndef BTN_BG_ITEM_H
#define BTN_BG_ITEM_H

#include "btn_bg_map_item.h"
#include "btn_palette_item.h"
#include "btn_bg_tiles_item.h"

namespace btn
{

class bg_item
{

public:
    constexpr bg_item(const span<const tile>& tiles, const bg_map_cell& map_cells_ref, const size& map_dimensions,
                      const span<const color>& palette) :
        bg_item(bg_tiles_item(tiles), bg_map_item(map_cells_ref, map_dimensions), palette_item(palette))
    {
    }

    constexpr bg_item(const bg_tiles_item& tiles_item, const bg_map_item& map_item, const palette_item& palette_item) :
        _tiles_item(tiles_item),
        _map_item(map_item),
        _palette_item(palette_item)
    {
        BTN_CONSTEXPR_ASSERT(tiles_item.valid_tiles_count(palette_item.eight_bits_per_pixel()), "Invalid tiles count");
    }

    [[nodiscard]] constexpr const bg_tiles_item& tiles() const
    {
        return _tiles_item;
    }

    [[nodiscard]] constexpr const bg_map_item& map() const
    {
        return _map_item;
    }

    [[nodiscard]] constexpr const palette_item& palette() const
    {
        return _palette_item;
    }

    [[nodiscard]] constexpr friend bool operator==(const bg_item& a, const bg_item& b)
    {
        return a._tiles_item == b._tiles_item && a._map_item == b._map_item && a._palette_item == b._palette_item;
    }

    [[nodiscard]] constexpr friend bool operator!=(const bg_item& a, const bg_item& b)
    {
        return ! (a == b);
    }

private:
    bg_tiles_item _tiles_item;
    bg_map_item _map_item;
    palette_item _palette_item;
};

}

#endif

