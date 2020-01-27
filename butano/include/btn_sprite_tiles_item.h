#ifndef BTN_SPRITE_TILES_ITEM_H
#define BTN_SPRITE_TILES_ITEM_H

#include "btn_span.h"
#include "btn_tile.h"
#include "btn_optional_fwd.h"

namespace btn
{

class sprite_tiles_ptr;
enum class create_mode;

class sprite_tiles_item
{

public:
    constexpr sprite_tiles_item(const span<const tile>& tiles_ref, int graphics_count) :
        _tiles_ref(tiles_ref),
        _graphics_count(graphics_count)
    {
        BTN_CONSTEXPR_ASSERT(! tiles_ref.empty(), "Tiles ref is empty");
        BTN_CONSTEXPR_ASSERT(graphics_count > 0, "Invalid graphics count");
        BTN_CONSTEXPR_ASSERT(graphics_count <= int(tiles_ref.size()), "Invalid tiles or graphics count");
        BTN_CONSTEXPR_ASSERT(tiles_ref.size() % size_t(graphics_count) == 0, "Invalid tiles or graphics count");
    }

    [[nodiscard]] constexpr int graphics_count() const
    {
        return _graphics_count;
    }

    [[nodiscard]] constexpr int tiles_count_per_graphic() const
    {
        return int(_tiles_ref.size()) / _graphics_count;
    }

    [[nodiscard]] constexpr const span<const tile>& tiles_ref() const
    {
        return _tiles_ref;
    }

    [[nodiscard]] constexpr span<const tile> tiles_ref(int graphics_index) const
    {
        BTN_CONSTEXPR_ASSERT(graphics_index >= 0, "Invalid graphics index");
        BTN_CONSTEXPR_ASSERT(graphics_index < _graphics_count, "Invalid graphics index");

        auto tiles_size = size_t(tiles_count_per_graphic());
        return span<const tile>(_tiles_ref.data() + (size_t(graphics_index) * tiles_size), tiles_size);
    }

    [[nodiscard]] optional<sprite_tiles_ptr> create_tiles(int graphics_index, create_mode create_mode) const;

    [[nodiscard]] constexpr friend bool operator==(const sprite_tiles_item& a, const sprite_tiles_item& b)
    {
        return a._tiles_ref.data() == b._tiles_ref.data() && a._tiles_ref.size() == b._tiles_ref.size() &&
                a._graphics_count == b._graphics_count;
    }

    [[nodiscard]] constexpr friend bool operator!=(const sprite_tiles_item& a, const sprite_tiles_item& b)
    {
        return ! (a == b);
    }

private:
    span<const tile> _tiles_ref;
    int _graphics_count;
};

}

#endif
