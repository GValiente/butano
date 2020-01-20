#ifndef BTN_BG_MAP_ITEM_H
#define BTN_BG_MAP_ITEM_H

#include "btn_assert.h"
#include "btn_bg_map_cell.h"
#include "btn_optional_fwd.h"

namespace btn
{

class bg_map_ptr;
enum class create_mode;

class bg_map_item
{

public:
    constexpr bg_map_item(const bg_map_cell& cells_ref, int width, int height) :
        _cells_ptr(&cells_ref),
        _width(uint16_t(width)),
        _height(uint16_t(height))
    {
        BTN_CONSTEXPR_ASSERT(width == 32 || width == 64, "Invalid width");
        BTN_CONSTEXPR_ASSERT(height == 32 || height == 64, "Invalid height");
    }

    [[nodiscard]] constexpr const bg_map_cell& cells_ref() const
    {
        return *_cells_ptr;
    }

    [[nodiscard]] constexpr int width() const
    {
        return _width;
    }

    [[nodiscard]] constexpr int height() const
    {
        return _height;
    }

    [[nodiscard]] optional<bg_map_ptr> create_map_ptr(create_mode create_mode) const;

    [[nodiscard]] constexpr friend bool operator==(const bg_map_item& a, const bg_map_item& b)
    {
        return a._cells_ptr == b._cells_ptr && a._width == b._width && a._height == b._height;
    }

    [[nodiscard]] constexpr friend bool operator!=(const bg_map_item& a, const bg_map_item& b)
    {
        return ! (a == b);
    }

private:
    const bg_map_cell* _cells_ptr;
    uint16_t _width;
    uint16_t _height;
};

}

#endif
