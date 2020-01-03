#ifndef BTN_SPRITE_ITEM_H
#define BTN_SPRITE_ITEM_H

#include "btn_sprite_shape_size.h"
#include "btn_sprite_tiles_item.h"
#include "btn_sprite_palette_item.h"

namespace btn
{

class sprite_item
{

public:
    constexpr sprite_item(sprite_shape shape, sprite_size size, const span<const tile>& tiles,
                          const span<const color>& palette, int graphics) :
        sprite_item(shape, size, sprite_tiles_item(tiles, graphics), sprite_palette_item(palette))
    {
    }

    constexpr sprite_item(sprite_shape shape, sprite_size size, const sprite_tiles_item& tiles_item,
                          const sprite_palette_item& palette_item) :
        _shape(shape),
        _size(size),
        _tiles_item(tiles_item),
        _palette_item(palette_item)
    {
        BTN_CONSTEXPR_ASSERT(int(tiles_item.tiles().size()) ==
                             shape_size().tiles(palette_item.palette().size() > 16) * tiles_item.graphics(),
                             "Invalid shape or size");
    }

    [[nodiscard]] constexpr sprite_shape shape() const
    {
        return _shape;
    }

    [[nodiscard]] constexpr sprite_size size() const
    {
        return _size;
    }

    [[nodiscard]] constexpr sprite_shape_size shape_size() const
    {
        return sprite_shape_size(_shape, _size);
    }

    [[nodiscard]] constexpr const sprite_tiles_item& tiles_item() const
    {
        return _tiles_item;
    }

    [[nodiscard]] constexpr const sprite_palette_item& palette_item() const
    {
        return _palette_item;
    }

    [[nodiscard]] constexpr bool operator==(const sprite_item& other) const
    {
        return _shape == other._shape && _size == other._size && _tiles_item == other._tiles_item &&
                _palette_item == other._palette_item;
    }

    [[nodiscard]] constexpr bool operator!=(const sprite_item& other) const
    {
        return ! (*this == other);
    }

private:
    sprite_shape _shape;
    sprite_size _size;
    sprite_tiles_item _tiles_item;
    sprite_palette_item _palette_item;
};

}

#endif

