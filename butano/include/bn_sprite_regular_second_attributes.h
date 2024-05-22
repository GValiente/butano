/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_REGULAR_SECOND_ATTRIBUTES_H
#define BN_SPRITE_REGULAR_SECOND_ATTRIBUTES_H

/**
 * @file
 * bn::sprite_regular_second_attributes header file.
 *
 * @ingroup sprite
 */

#include "bn_fixed.h"

namespace bn
{

/**
 * @brief Manages the attributes to commit to the second GBA register of a regular sprite
 * (without an attached sprite_affine_mat_ptr).
 *
 * @ingroup sprite
 */
class sprite_regular_second_attributes
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr sprite_regular_second_attributes() = default;

    /**
     * @brief Constructor.
     * @param x Horizontal position of a sprite (relative to its camera, if it has one).
     * @param horizontal_flip Indicates if the sprite is flipped in the horizontal axis or not.
     * @param vertical_flip Indicates if the sprite is flipped in the vertical axis or not.
     */
    constexpr sprite_regular_second_attributes(fixed x, bool horizontal_flip, bool vertical_flip) :
        _x(x),
        _horizontal_flip(horizontal_flip),
        _vertical_flip(vertical_flip)
    {
    }

    /**
     * @brief Returns the horizontal position of a sprite (relative to its camera, if it has one).
     */
    [[nodiscard]] constexpr fixed x() const
    {
        return _x;
    }

    /**
     * @brief Sets the horizontal position of a sprite (relative to its camera, if it has one).
     */
    constexpr void set_x(fixed x)
    {
        _x = x;
    }

    /**
     * @brief Indicates if the sprite is flipped in the horizontal axis or not.
     */
    [[nodiscard]] constexpr bool horizontal_flip() const
    {
        return _horizontal_flip;
    }

    /**
     * @brief Sets if the sprite is flipped in the horizontal axis or not.
     */
    constexpr void set_horizontal_flip(bool horizontal_flip)
    {
        _horizontal_flip = horizontal_flip;
    }

    /**
     * @brief Indicates if the sprite is flipped in the vertical axis or not.
     */
    [[nodiscard]] constexpr bool vertical_flip() const
    {
        return _vertical_flip;
    }

    /**
     * @brief Sets if the sprite is flipped in the vertical axis or not.
     */
    constexpr void set_vertical_flip(bool vertical_flip)
    {
        _vertical_flip = vertical_flip;
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const sprite_regular_second_attributes& a,
                                                   const sprite_regular_second_attributes& b) = default;

private:
    fixed _x = 0;
    bool _horizontal_flip = false;
    bool _vertical_flip = false;
};

}

#endif

