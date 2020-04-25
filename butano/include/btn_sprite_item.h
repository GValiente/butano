#ifndef BTN_SPRITE_ITEM_H
#define BTN_SPRITE_ITEM_H

#include "btn_sprite_shape_size.h"
#include "btn_sprite_tiles_item.h"
#include "btn_sprite_palette_item.h"

namespace btn
{

class sprite_ptr;
class fixed_point;

class sprite_item
{

public:
    constexpr sprite_item(const sprite_shape_size& shape_size, const span<const tile>& tiles,
                          const span<const color>& palette, palette_bpp_mode bpp_mode, int graphics_count) :
        sprite_item(shape_size, sprite_tiles_item(tiles, graphics_count), sprite_palette_item(palette, bpp_mode))
    {
    }

    constexpr sprite_item(const sprite_shape_size& shape_size, const sprite_tiles_item& tiles_item,
                          const sprite_palette_item& palette_item) :
        _shape_size(shape_size),
        _tiles_item(tiles_item),
        _palette_item(palette_item)
    {
        BTN_CONSTEXPR_ASSERT(tiles_item.tiles_ref().size() ==
                             _shape_size.tiles_count(palette_item.bpp_mode()) * tiles_item.graphics_count(),
                             "Invalid shape or size");
    }

    [[nodiscard]] constexpr const sprite_shape_size& shape_size() const
    {
        return _shape_size;
    }

    [[nodiscard]] constexpr const sprite_tiles_item& tiles_item() const
    {
        return _tiles_item;
    }

    [[nodiscard]] constexpr const sprite_palette_item& palette_item() const
    {
        return _palette_item;
    }

    [[nodiscard]] sprite_ptr create_sprite(fixed x, fixed y) const;

    [[nodiscard]] sprite_ptr create_sprite(fixed x, fixed y, int graphics_index) const;

    [[nodiscard]] sprite_ptr create_sprite(const fixed_point& position) const;

    [[nodiscard]] sprite_ptr create_sprite(const fixed_point& position, int graphics_index) const;

    [[nodiscard]] optional<sprite_ptr> optional_create_sprite(fixed x, fixed y) const;

    [[nodiscard]] optional<sprite_ptr> optional_create_sprite(fixed x, fixed y, int graphics_index) const;

    [[nodiscard]] optional<sprite_ptr> optional_create_sprite(const fixed_point& position) const;

    [[nodiscard]] optional<sprite_ptr> optional_create_sprite(const fixed_point& position, int graphics_index) const;

    [[nodiscard]] constexpr friend bool operator==(const sprite_item& a, const sprite_item& b)
    {
        return a._shape_size == b._shape_size && a._tiles_item == b._tiles_item && a._palette_item == b._palette_item;
    }

    [[nodiscard]] constexpr friend bool operator!=(const sprite_item& a, const sprite_item& b)
    {
        return ! (a == b);
    }

private:
    sprite_shape_size _shape_size;
    sprite_tiles_item _tiles_item;
    sprite_palette_item _palette_item;
};

}

#endif

