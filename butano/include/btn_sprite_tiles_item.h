#ifndef BTN_SPRITE_TILES_ITEM_H
#define BTN_SPRITE_TILES_ITEM_H

#include "btn_span.h"
#include "btn_tile.h"

namespace btn
{

class sprite_tiles_ptr;
enum class create_mode;

class sprite_tiles_item
{

public:
    constexpr sprite_tiles_item(const span<const tile>& tiles, int graphics) :
        _tiles(tiles),
        _graphics(graphics)
    {
        BTN_CONSTEXPR_ASSERT(! tiles.empty(), "Tiles is empty");
        BTN_CONSTEXPR_ASSERT(graphics > 0, "Invalid graphics");
        BTN_CONSTEXPR_ASSERT(graphics <= int(tiles.size()), "Invalid tiles or graphics");
        BTN_CONSTEXPR_ASSERT(int(tiles.size()) % graphics == 0, "Invalid tiles or graphics");
    }

    [[nodiscard]] constexpr const span<const tile>& tiles() const
    {
        return _tiles;
    }

    [[nodiscard]] constexpr int graphics() const
    {
        return _graphics;
    }

    [[nodiscard]] constexpr int tiles_per_graphic() const
    {
        return int(_tiles.size()) / _graphics;
    }

    [[nodiscard]] constexpr span<const tile> tiles_ref(int graphics_id) const
    {
        BTN_CONSTEXPR_ASSERT(graphics_id >= 0, "Invalid graphics_id");
        BTN_CONSTEXPR_ASSERT(graphics_id < _graphics, "Invalid graphics_id");

        auto tiles_size = size_t(tiles_per_graphic());
        return span<const tile>(_tiles.data() + (size_t(graphics_id) * tiles_size), tiles_size);
    }

    [[nodiscard]] sprite_tiles_ptr tiles_ptr(int graphics_id, create_mode create_mode) const;

    [[nodiscard]] constexpr friend bool operator==(const sprite_tiles_item& a, const sprite_tiles_item& b)
    {
        return a._tiles.data() == b._tiles.data() && a._tiles.size() == b._tiles.size() &&
                a._graphics == b._graphics;
    }

    [[nodiscard]] constexpr friend bool operator!=(const sprite_tiles_item& a, const sprite_tiles_item& b)
    {
        return ! (a == b);
    }

private:
    span<const tile> _tiles;
    int _graphics;
};

}

#endif
