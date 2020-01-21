#ifndef BTN_BG_TILES_ITEM_H
#define BTN_BG_TILES_ITEM_H

#include "btn_span.h"
#include "btn_tile.h"
#include "btn_optional_fwd.h"

namespace btn
{

class bg_tiles_ptr;
enum class create_mode;

class bg_tiles_item
{

public:
    constexpr explicit bg_tiles_item(const span<const tile>& tiles) :
        _tiles(tiles)
    {
        BTN_CONSTEXPR_ASSERT(valid_tiles_count(false) || valid_tiles_count(true), "Invalid tiles count");
    }

    [[nodiscard]] constexpr const span<const tile>& tiles() const
    {
        return _tiles;
    }

    [[nodiscard]] constexpr bool valid_tiles_count(bool eight_bits_per_pixel) const
    {
        size_t count = _tiles.size();

        if(eight_bits_per_pixel)
        {
            return count && count < 2048 && (count % 2) == 0;
        }

        return count && count < 1024;
    }

    [[nodiscard]] optional<bg_tiles_ptr> create_tiles_ptr(create_mode create_mode) const;

    [[nodiscard]] constexpr friend bool operator==(const bg_tiles_item& a, const bg_tiles_item& b)
    {
        return a._tiles.data() == b._tiles.data() && a._tiles.size() == b._tiles.size();
    }

    [[nodiscard]] constexpr friend bool operator!=(const bg_tiles_item& a, const bg_tiles_item& b)
    {
        return ! (a == b);
    }

private:
    span<const tile> _tiles;
};

}

#endif
