#ifndef BTN_SPRITE_TILES_ITEM_H
#define BTN_SPRITE_TILES_ITEM_H

#include "btn_span.h"
#include "btn_tile.h"
#include "btn_optional_fwd.h"

namespace btn
{

class sprite_tiles_ptr;

class sprite_tiles_item
{

public:
    constexpr sprite_tiles_item(const span<const tile>& tiles_ref, int graphics_count) :
        _tiles_ref(tiles_ref),
        _graphics_count(graphics_count),
        _tiles_count_per_graphic(0)
    {
        BTN_ASSERT(! tiles_ref.empty(), "Tiles ref is empty");
        BTN_ASSERT(graphics_count > 0, "Invalid graphics count: ", graphics_count);
        BTN_ASSERT(graphics_count <= tiles_ref.size(), "Invalid tiles or graphics count: ",
                   tiles_ref.size(), " - ", graphics_count);
        BTN_ASSERT(tiles_ref.size() % graphics_count == 0, "Invalid tiles or graphics count: ",
                   tiles_ref.size(), " - ", graphics_count);

        _tiles_count_per_graphic = tiles_ref.size() / graphics_count;
    }

    [[nodiscard]] constexpr int graphics_count() const
    {
        return _graphics_count;
    }

    [[nodiscard]] constexpr int tiles_count_per_graphic() const
    {
        return _tiles_count_per_graphic;
    }

    [[nodiscard]] constexpr const span<const tile>& tiles_ref() const
    {
        return _tiles_ref;
    }

    [[nodiscard]] constexpr span<const tile> graphics_tiles_ref() const
    {
        return span<const tile>(_tiles_ref.data(), _tiles_count_per_graphic);
    }

    [[nodiscard]] constexpr span<const tile> graphics_tiles_ref(int graphics_index) const
    {
        BTN_ASSERT(graphics_index >= 0, "Invalid graphics index: ", graphics_index);
        BTN_ASSERT(graphics_index < _graphics_count, "Invalid graphics index: ",
                   graphics_index, " - ", _graphics_count);

        int tiles_count = _tiles_count_per_graphic;
        return span<const tile>(_tiles_ref.data() + (graphics_index * tiles_count), tiles_count);
    }

    [[nodiscard]] optional<sprite_tiles_ptr> find_tiles() const;

    [[nodiscard]] optional<sprite_tiles_ptr> find_tiles(int graphics_index) const;

    [[nodiscard]] sprite_tiles_ptr create_tiles() const;

    [[nodiscard]] sprite_tiles_ptr create_tiles(int graphics_index) const;

    [[nodiscard]] sprite_tiles_ptr create_new_tiles() const;

    [[nodiscard]] sprite_tiles_ptr create_new_tiles(int graphics_index) const;

    [[nodiscard]] optional<sprite_tiles_ptr> create_tiles_optional() const;

    [[nodiscard]] optional<sprite_tiles_ptr> create_tiles_optional(int graphics_index) const;

    [[nodiscard]] optional<sprite_tiles_ptr> create_new_tiles_optional() const;

    [[nodiscard]] optional<sprite_tiles_ptr> create_new_tiles_optional(int graphics_index) const;

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
    int _tiles_count_per_graphic;
};

}

#endif
