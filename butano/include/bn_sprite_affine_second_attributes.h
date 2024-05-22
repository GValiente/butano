/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_AFFINE_SECOND_ATTRIBUTES_H
#define BN_SPRITE_AFFINE_SECOND_ATTRIBUTES_H

/**
 * @file
 * bn::sprite_affine_second_attributes header file.
 *
 * @ingroup sprite
 */

#include "bn_fixed.h"
#include "bn_sprite_affine_mat_ptr.h"

namespace bn
{

/**
 * @brief Manages the attributes to commit to the second GBA register of an affine sprite
 * (with an attached sprite_affine_mat_ptr).
 *
 * @ingroup sprite
 */
class sprite_affine_second_attributes
{

public:
    /**
     * @brief Constructor.
     * @param x Horizontal position of a sprite (relative to its camera, if it has one).
     * @param affine_mat Reference to the sprite_affine_mat_ptr attached to a sprite.
     */
    sprite_affine_second_attributes(fixed x, const sprite_affine_mat_ptr& affine_mat);

    /**
     * @brief Constructor.
     * @param x Horizontal position of a sprite (relative to its camera, if it has one).
     * @param affine_mat Moved reference to the sprite_affine_mat_ptr attached to a sprite.
     */
    sprite_affine_second_attributes(fixed x, sprite_affine_mat_ptr&& affine_mat);

    /**
     * @brief Returns the horizontal position of a sprite (relative to its camera, if it has one).
     */
    [[nodiscard]] fixed x() const
    {
        return _x;
    }

    /**
     * @brief Sets the horizontal position of a sprite (relative to its camera, if it has one).
     */
    void set_x(fixed x)
    {
        _x = x;
    }

    /**
     * @brief Returns the sprite_affine_mat_ptr attached to a sprite.
     */
    [[nodiscard]] const sprite_affine_mat_ptr& affine_mat() const
    {
        return _affine_mat;
    }

    /**
     * @brief Sets the sprite_affine_mat_ptr attached to a sprite.
     * @param affine_mat sprite_affine_mat_ptr to copy.
     */
    void set_affine_mat(const sprite_affine_mat_ptr& affine_mat)
    {
        _affine_mat = affine_mat;
    }

    /**
     * @brief Sets the sprite_affine_mat_ptr attached to a sprite.
     * @param affine_mat sprite_affine_mat_ptr to move.
     */
    void set_affine_mat(sprite_affine_mat_ptr&& affine_mat)
    {
        _affine_mat = move(affine_mat);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] friend bool operator==(const sprite_affine_second_attributes& a,
                                         const sprite_affine_second_attributes& b) = default;

private:
    fixed _x;
    sprite_affine_mat_ptr _affine_mat;
};

}

#endif

