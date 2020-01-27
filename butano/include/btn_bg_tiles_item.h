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
    constexpr explicit bg_tiles_item(const span<const tile>& tiles_ref) :
        _tiles_ref(tiles_ref)
    {
        BTN_CONSTEXPR_ASSERT(valid_tiles_count(false) || valid_tiles_count(true), "Invalid tiles count");
    }

    [[nodiscard]] constexpr const span<const tile>& tiles_ref() const
    {
        return _tiles_ref;
    }

    [[nodiscard]] constexpr bool valid_tiles_count(bool eight_bits_per_pixel) const
    {
        size_t count = _tiles_ref.size();

        if(eight_bits_per_pixel)
        {
            return count && count < 2048 && (count % 2) == 0;
        }

        return count && count < 1024;
    }

    [[nodiscard]] optional<bg_tiles_ptr> create_tiles(create_mode create_mode) const;

    [[nodiscard]] constexpr friend bool operator==(const bg_tiles_item& a, const bg_tiles_item& b)
    {
        return a._tiles_ref.data() == b._tiles_ref.data() && a._tiles_ref.size() == b._tiles_ref.size();
    }

    [[nodiscard]] constexpr friend bool operator!=(const bg_tiles_item& a, const bg_tiles_item& b)
    {
        return ! (a == b);
    }

private:
    span<const tile> _tiles_ref;
};

}

#endif
