/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_REGULAR_BG_MAP_ITEM_H
#define BTN_REGULAR_BG_MAP_ITEM_H

#include "btn_size.h"
#include "btn_optional_fwd.h"
#include "btn_regular_bg_map_cell.h"

namespace btn
{

class bg_tiles_ptr;
class bg_palette_ptr;
class regular_bg_map_ptr;

class regular_bg_map_item
{

public:
    constexpr regular_bg_map_item(const regular_bg_map_cell& cells_ref, const size& dimensions) :
        _cells_ptr(&cells_ref),
        _dimensions(dimensions)
    {
        BTN_ASSERT(dimensions.width() == 32 || dimensions.width() == 64, "Invalid width: ", dimensions.width());
        BTN_ASSERT(dimensions.height() == 32 || dimensions.height() == 64, "Invalid height: ", dimensions.height());
    }

    [[nodiscard]] constexpr const regular_bg_map_cell& cells_ref() const
    {
        return *_cells_ptr;
    }

    [[nodiscard]] constexpr const size& dimensions() const
    {
        return _dimensions;
    }

    [[nodiscard]] optional<regular_bg_map_ptr> find_map(const bg_tiles_ptr& tiles, const bg_palette_ptr& palette) const;

    [[nodiscard]] regular_bg_map_ptr create_map(bg_tiles_ptr tiles, bg_palette_ptr palette) const;

    [[nodiscard]] regular_bg_map_ptr create_new_map(bg_tiles_ptr tiles, bg_palette_ptr palette_ptr) const;

    [[nodiscard]] optional<regular_bg_map_ptr> create_map_optional(bg_tiles_ptr tiles, bg_palette_ptr palette) const;

    [[nodiscard]] optional<regular_bg_map_ptr> create_new_map_optional(bg_tiles_ptr tiles, bg_palette_ptr palette) const;

    [[nodiscard]] constexpr friend bool operator==(const regular_bg_map_item& a, const regular_bg_map_item& b) = default;

private:
    const regular_bg_map_cell* _cells_ptr;
    size _dimensions;
};

}

#endif
