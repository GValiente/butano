#ifndef BTN_REGULAR_BG_MAP_ITEM_H
#define BTN_REGULAR_BG_MAP_ITEM_H

#include "btn_size.h"
#include "btn_create_mode.h"
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
        BTN_CONSTEXPR_ASSERT(dimensions.width() == 32 || dimensions.width() == 64, "Invalid width");
        BTN_CONSTEXPR_ASSERT(dimensions.height() == 32 || dimensions.height() == 64, "Invalid height");
    }

    [[nodiscard]] constexpr const regular_bg_map_cell& cells_ref() const
    {
        return *_cells_ptr;
    }

    [[nodiscard]] constexpr const size& dimensions() const
    {
        return _dimensions;
    }

    [[nodiscard]] regular_bg_map_ptr create_map(bg_tiles_ptr tiles_ptr, bg_palette_ptr palette_ptr,
                                                create_mode create_mode = create_mode::FIND_OR_CREATE) const;

    [[nodiscard]] optional<regular_bg_map_ptr> optional_create_map(
            bg_tiles_ptr tiles_ptr, bg_palette_ptr palette_ptr,
            create_mode create_mode = create_mode::FIND_OR_CREATE) const;

    [[nodiscard]] constexpr friend bool operator==(const regular_bg_map_item& a, const regular_bg_map_item& b)
    {
        return a._cells_ptr == b._cells_ptr && a._dimensions == b._dimensions;
    }

    [[nodiscard]] constexpr friend bool operator!=(const regular_bg_map_item& a, const regular_bg_map_item& b)
    {
        return ! (a == b);
    }

private:
    const regular_bg_map_cell* _cells_ptr;
    size _dimensions;
};

}

#endif
