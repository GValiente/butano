/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_THIRD_ATTRIBUTES_H
#define BN_SPRITE_THIRD_ATTRIBUTES_H

/**
 * @file
 * bn::sprite_third_attributes header file.
 *
 * @ingroup sprite
 */

#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_palette_ptr.h"

namespace bn
{

/**
 * @brief Manages the attributes to commit to the third GBA register of a sprite.
 *
 * @ingroup sprite
 */
class sprite_third_attributes
{

public:
    /**
     * @brief Constructor.
     * @param tiles Smart pointer to a sprite tile set.
     * @param palette Smart pointer to a sprite color palette.
     * @param bg_priority Drawing priority relative to backgrounds in the range [0..3].
     *
     * Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
     * Sprites cover backgrounds of the same priority.
     */
    sprite_third_attributes(sprite_tiles_ptr tiles, sprite_palette_ptr palette, int bg_priority);

    /**
     * @brief Returns the smart pointer to a sprite tile set.
     */
    [[nodiscard]] const sprite_tiles_ptr& tiles() const
    {
        return _tiles;
    }

    /**
     * @brief Sets the smart pointer to a sprite tile set.
     * @param tiles Smart pointer to copy.
     */
    void set_tiles(const sprite_tiles_ptr& tiles)
    {
        _tiles = tiles;
    }

    /**
     * @brief Sets the smart pointer to a sprite tile set.
     * @param tiles Smart pointer to move.
     */
    void set_tiles(sprite_tiles_ptr&& tiles)
    {
        _tiles = move(tiles);
    }

    /**
     * @brief Returns the smart pointer to a sprite color palette.
     */
    [[nodiscard]] const sprite_palette_ptr& palette() const
    {
        return _palette;
    }

    /**
     * @brief Sets the smart pointer to a sprite color palette.
     * @param palette Smart pointer to copy.
     */
    void set_palette(const sprite_palette_ptr& palette)
    {
        _palette = palette;
    }

    /**
     * @brief Sets the smart pointer to a sprite color palette.
     * @param palette Smart pointer to move.
     */
    void set_palette(sprite_palette_ptr&& palette)
    {
        _palette = move(palette);
    }

    /**
     * @brief Returns the priority relative to backgrounds.
     *
     * Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
     * Sprites cover backgrounds of the same priority.
     */
    [[nodiscard]] int bg_priority() const
    {
        return _bg_priority;
    }

    /**
     * @brief Sets the priority relative to backgrounds.
     *
     * Sprites with higher priority are drawn first (and therefore can be covered by later sprites and backgrounds).
     * Sprites cover backgrounds of the same priority.
     *
     * @param bg_priority Priority relative to backgrounds in the range [0..3].
     */
    void set_bg_priority(int bg_priority);

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const sprite_third_attributes& a, const sprite_third_attributes& b) = default;

private:
    sprite_tiles_ptr _tiles;
    sprite_palette_ptr _palette;
    int8_t _bg_priority;
};

}

#endif

