/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPRITE_ITEM_H
#define BTN_SPRITE_ITEM_H

#include "btn_fixed_fwd.h"
#include "btn_sprite_shape_size.h"
#include "btn_sprite_tiles_item.h"
#include "btn_sprite_palette_item.h"

namespace btn
{

class sprite_ptr;
class fixed_point;

/**
 * @brief Contains the required information to generate sprites.
 *
 * The assets conversion tools generate an object of this type in the build folder for each *.bmp file.
 *
 * Tiles and colors are not copied but referenced, so they should outlive sprite_item to avoid dangling references.
 *
 * @ingroup sprite
 * @ingroup tool
 */
class sprite_item
{

public:
    /**
     * @brief Constructor.
     * @param shape_size Shape and size of output sprites.
     * @param tiles_ref Reference to one or more sprite tile sets.
     *
     * The tiles are not copied but referenced, so they should outlive sprite_item to avoid dangling references.
     *
     * @param colors_ref Reference to an array of multiples of 16 colors.
     *
     * The colors are not copied but referenced, so they should outlive sprite_item to avoid dangling references.
     *
     * @param bpp_mode Bits per pixel of output sprites.
     * @param graphics_count Number of sprite tile sets contained in tiles_ref.
     */
    constexpr sprite_item(const sprite_shape_size& shape_size, const span<const tile>& tiles_ref,
                          const span<const color>& colors_ref, palette_bpp_mode bpp_mode, int graphics_count) :
        sprite_item(shape_size, sprite_tiles_item(tiles_ref, graphics_count),
                    sprite_palette_item(colors_ref, bpp_mode))
    {
    }

    /**
     * @brief Constructor.
     * @param shape_size Shape and size of output sprites.
     * @param tiles_item It creates the tiles of output sprites.
     * @param palette_item It creates the color palette of output sprites.
     */
    constexpr sprite_item(const sprite_shape_size& shape_size, const sprite_tiles_item& tiles_item,
                          const sprite_palette_item& palette_item) :
        _shape_size(shape_size),
        _tiles_item(tiles_item),
        _palette_item(palette_item)
    {
        BTN_ASSERT(tiles_item.tiles_ref().size() ==
                   _shape_size.tiles_count(palette_item.bpp_mode()) * tiles_item.graphics_count(),
                   "Invalid shape or size");
    }

    /**
     * @brief Returns the shape and size of output sprites.
     */
    [[nodiscard]] constexpr const sprite_shape_size& shape_size() const
    {
        return _shape_size;
    }

    /**
     * @brief Returns the item used to create the tiles of output sprites.
     */
    [[nodiscard]] constexpr const sprite_tiles_item& tiles_item() const
    {
        return _tiles_item;
    }

    /**
     * @brief Returns the item used to create the color palette of output sprites.
     */
    [[nodiscard]] constexpr const sprite_palette_item& palette_item() const
    {
        return _palette_item;
    }

    /**
     * @brief Creates a sprite_ptr using the information contained in this item.
     * @param x Horizontal position of the sprite.
     * @param y Vertical position of the sprite.
     * @return The requested sprite_ptr.
     */
    [[nodiscard]] sprite_ptr create_sprite(fixed x, fixed y) const;

    /**
     * @brief Creates a sprite_ptr using the information contained in this item.
     * @param x Horizontal position of the sprite.
     * @param y Vertical position of the sprite.
     * @param graphics_index Index of the tile set to reference in tiles_item().
     * @return The requested sprite_ptr.
     */
    [[nodiscard]] sprite_ptr create_sprite(fixed x, fixed y, int graphics_index) const;

    /**
     * @brief Creates a sprite_ptr using the information contained in this item.
     * @param position Position of the sprite.
     * @return The requested sprite_ptr.
     */
    [[nodiscard]] sprite_ptr create_sprite(const fixed_point& position) const;

    /**
     * @brief Creates a sprite_ptr using the information contained in this item.
     * @param position Position of the sprite.
     * @param graphics_index Index of the tile set to reference in tiles_item().
     * @return The requested sprite_ptr.
     */
    [[nodiscard]] sprite_ptr create_sprite(const fixed_point& position, int graphics_index) const;

    /**
     * @brief Creates a sprite_ptr using the information contained in this item.
     * @param x Horizontal position of the sprite.
     * @param y Vertical position of the sprite.
     * @return The requested sprite_ptr if it can be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] optional<sprite_ptr> create_sprite_optional(fixed x, fixed y) const;

    /**
     * @brief Creates a sprite_ptr using the information contained in this item.
     * @param x Horizontal position of the sprite.
     * @param y Vertical position of the sprite.
     * @param graphics_index Index of the tile set to reference in tiles_item().
     * @return The requested sprite_ptr if it can be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] optional<sprite_ptr> create_sprite_optional(fixed x, fixed y, int graphics_index) const;

    /**
     * @brief Creates a sprite_ptr using the information contained in this item.
     * @param position Position of the sprite.
     * @return The requested sprite_ptr if it can be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] optional<sprite_ptr> create_sprite_optional(const fixed_point& position) const;

    /**
     * @brief Creates a sprite_ptr using the information contained in this item.
     * @param position Position of the sprite.
     * @param graphics_index Index of the tile set to reference in tiles_item().
     * @return The requested sprite_ptr if it can be allocated; <b>nullopt</b> otherwise.
     */
    [[nodiscard]] optional<sprite_ptr> create_sprite_optional(const fixed_point& position, int graphics_index) const;

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const sprite_item& a, const sprite_item& b) = default;

private:
    sprite_shape_size _shape_size;
    sprite_tiles_item _tiles_item;
    sprite_palette_item _palette_item;
};

}

#endif

