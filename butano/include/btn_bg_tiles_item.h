#ifndef BTN_BG_TILES_ITEM_H
#define BTN_BG_TILES_ITEM_H

#include "btn_span.h"
#include "btn_tile.h"
#include "btn_optional_fwd.h"
#include "btn_palette_bpp_mode.h"

namespace btn
{

class bg_tiles_ptr;

class bg_tiles_item
{

public:
    constexpr explicit bg_tiles_item(const span<const tile>& tiles_ref) :
        _tiles_ref(tiles_ref)
    {
        BTN_ASSERT(valid_tiles_count(palette_bpp_mode::BPP_4) || valid_tiles_count(palette_bpp_mode::BPP_8),
                   "Invalid tiles count: ", _tiles_ref.size());
    }

    [[nodiscard]] constexpr const span<const tile>& tiles_ref() const
    {
        return _tiles_ref;
    }

    [[nodiscard]] constexpr bool valid_tiles_count(palette_bpp_mode bpp_mode) const
    {
        int count = _tiles_ref.size();

        if(bpp_mode == palette_bpp_mode::BPP_8)
        {
            return count && count < 2048 && (count % 2) == 0;
        }

        return count && count < 1024;
    }

    [[nodiscard]] optional<bg_tiles_ptr> find_tiles() const;

    [[nodiscard]] bg_tiles_ptr create_tiles() const;

    [[nodiscard]] bg_tiles_ptr create_new_tiles() const;

    [[nodiscard]] optional<bg_tiles_ptr> create_tiles_optional() const;

    [[nodiscard]] optional<bg_tiles_ptr> create_new_tiles_optional() const;

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
