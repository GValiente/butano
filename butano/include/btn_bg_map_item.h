#ifndef BTN_BG_MAP_ITEM_H
#define BTN_BG_MAP_ITEM_H

#include "btn_size.h"
#include "btn_bg_map_cell.h"
#include "btn_optional_fwd.h"

namespace btn
{

class bg_map_ptr;
enum class create_mode;

class bg_map_item
{

public:
    constexpr bg_map_item(const bg_map_cell& cells_ref, const size& dimensions) :
        _cells_ptr(&cells_ref),
        _dimensions(dimensions)
    {
        BTN_CONSTEXPR_ASSERT(dimensions.width() == 32 || dimensions.width() == 64, "Invalid width");
        BTN_CONSTEXPR_ASSERT(dimensions.height() == 32 || dimensions.height() == 64, "Invalid height");
    }

    [[nodiscard]] constexpr const bg_map_cell& cells_ref() const
    {
        return *_cells_ptr;
    }

    [[nodiscard]] constexpr const size& dimensions() const
    {
        return _dimensions;
    }

    [[nodiscard]] optional<bg_map_ptr> create_map_ptr(create_mode create_mode) const;

    [[nodiscard]] constexpr friend bool operator==(const bg_map_item& a, const bg_map_item& b)
    {
        return a._cells_ptr == b._cells_ptr && a._dimensions == b._dimensions;
    }

    [[nodiscard]] constexpr friend bool operator!=(const bg_map_item& a, const bg_map_item& b)
    {
        return ! (a == b);
    }

private:
    const bg_map_cell* _cells_ptr;
    size _dimensions;
};

}

#endif
