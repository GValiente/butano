/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_MOSAIC_ATTRIBUTES_H
#define BN_MOSAIC_ATTRIBUTES_H

/**
 * @file
 * bn::mosaic_attributes header file.
 *
 * @ingroup mosaic
 */

#include "bn_fixed.h"
#include "bn_assert.h"

namespace bn
{

/**
 * @brief Manages the attributes to commit to the GBA mosaic register.
 *
 * @ingroup mosaic
 */
class mosaic_attributes
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr mosaic_attributes() = default;

    /**
     * @brief Constructor.
     * @param sprites_horizontal_stretch Horizontal stretch of the mosaic applied to the sprites, in the range [0..1].
     * @param sprites_vertical_stretch Vertical stretch of the mosaic applied to the sprites, in the range [0..1].
     * @param bgs_horizontal_stretch Horizontal stretch of the mosaic applied to the backgrounds, in the range [0..1].
     * @param bgs_vertical_stretch Vertical stretch of the mosaic applied to the backgrounds, in the range [0..1].
     */
    constexpr mosaic_attributes(fixed sprites_horizontal_stretch, fixed sprites_vertical_stretch,
                                fixed bgs_horizontal_stretch, fixed bgs_vertical_stretch) :
        _sprites_horizontal_stretch(sprites_horizontal_stretch),
        _sprites_vertical_stretch(sprites_vertical_stretch),
        _bgs_horizontal_stretch(bgs_horizontal_stretch),
        _bgs_vertical_stretch(bgs_vertical_stretch)
    {
        BN_ASSERT(sprites_horizontal_stretch >= 0 && sprites_horizontal_stretch <= 1,
                   "Invalid sprites horizontal stretch: ", sprites_horizontal_stretch);
        BN_ASSERT(sprites_vertical_stretch >= 0 && sprites_vertical_stretch <= 1,
                   "Invalid sprites vertical stretch: ", sprites_vertical_stretch);
        BN_ASSERT(bgs_horizontal_stretch >= 0 && bgs_horizontal_stretch <= 1,
                   "Invalid bgs horizontal stretch: ", bgs_horizontal_stretch);
        BN_ASSERT(bgs_vertical_stretch >= 0 && bgs_vertical_stretch <= 1,
                   "Invalid bgs vertical stretch: ", bgs_vertical_stretch);
    }

    /**
     * @brief Returns the horizontal stretch of the mosaic applied to the sprites.
     */
    [[nodiscard]] constexpr fixed sprites_horizontal_stretch() const
    {
        return _sprites_horizontal_stretch;
    }

    /**
     * @brief Sets the horizontal stretch of the mosaic applied to the sprites.
     * @param horizontal_stretch Horizontal stretch in the range [0..1].
     */
    constexpr void set_sprites_horizontal_stretch(fixed horizontal_stretch)
    {
        BN_ASSERT(horizontal_stretch >= 0 && horizontal_stretch <= 1,
                   "Invalid horizontal stretch: ", horizontal_stretch);

        _sprites_horizontal_stretch = horizontal_stretch;
    }

    /**
     * @brief Returns the vertical stretch of the mosaic applied to the sprites.
     */
    [[nodiscard]] constexpr fixed sprites_vertical_stretch() const
    {
        return _sprites_vertical_stretch;
    }

    /**
     * @brief Sets the vertical stretch of the mosaic applied to the sprites.
     * @param vertical_stretch Vertical stretch in the range [0..1].
     */
    constexpr void set_sprites_vertical_stretch(fixed vertical_stretch)
    {
        BN_ASSERT(vertical_stretch >= 0 && vertical_stretch <= 1, "Invalid vertical stretch: ", vertical_stretch);

        _sprites_vertical_stretch = vertical_stretch;
    }

    /**
     * @brief Sets the stretch of the mosaic applied to the sprites.
     * @param stretch Stretch in the range [0..1].
     */
    constexpr void set_sprites_stretch(fixed stretch)
    {
        BN_ASSERT(stretch >= 0 && stretch <= 1, "Invalid stretch: ", stretch);

        _sprites_horizontal_stretch = stretch;
        _sprites_vertical_stretch = stretch;
    }

    /**
     * @brief Sets the stretch of the mosaic applied to the sprites.
     * @param horizontal_stretch Horizontal stretch in the range [0..1].
     * @param vertical_stretch Vertical stretch in the range [0..1].
     */
    constexpr void set_sprites_stretch(fixed horizontal_stretch, fixed vertical_stretch)
    {
        BN_ASSERT(horizontal_stretch >= 0 && horizontal_stretch <= 1,
                   "Invalid horizontal stretch: ", horizontal_stretch);
        BN_ASSERT(vertical_stretch >= 0 && vertical_stretch <= 1, "Invalid vertical stretch: ", vertical_stretch);

        _sprites_horizontal_stretch = horizontal_stretch;
        _sprites_vertical_stretch = vertical_stretch;
    }

    /**
     * @brief Returns the horizontal stretch of the mosaic applied to the backgrounds.
     */
    [[nodiscard]] constexpr fixed bgs_horizontal_stretch() const
    {
        return _bgs_horizontal_stretch;
    }

    /**
     * @brief Sets the horizontal stretch of the mosaic applied to the backgrounds.
     * @param horizontal_stretch Horizontal stretch in the range [0..1].
     */
    constexpr void set_bgs_horizontal_stretch(fixed horizontal_stretch)
    {
        BN_ASSERT(horizontal_stretch >= 0 && horizontal_stretch <= 1,
                   "Invalid horizontal stretch: ", horizontal_stretch);

        _bgs_horizontal_stretch = horizontal_stretch;
    }

    /**
     * @brief Returns the vertical stretch of the mosaic applied to the backgrounds.
     */
    [[nodiscard]] constexpr fixed bgs_vertical_stretch() const
    {
        return _bgs_vertical_stretch;
    }

    /**
     * @brief Sets the vertical stretch of the mosaic applied to the backgrounds.
     * @param vertical_stretch Vertical stretch in the range [0..1].
     */
    constexpr void set_bgs_vertical_stretch(fixed vertical_stretch)
    {
        BN_ASSERT(vertical_stretch >= 0 && vertical_stretch <= 1, "Invalid vertical stretch: ", vertical_stretch);

        _bgs_vertical_stretch = vertical_stretch;
    }

    /**
     * @brief Sets the stretch of the mosaic applied to the backgrounds.
     * @param stretch Stretch in the range [0..1].
     */
    constexpr void set_bgs_stretch(fixed stretch)
    {
        BN_ASSERT(stretch >= 0 && stretch <= 1, "Invalid stretch: ", stretch);

        _bgs_horizontal_stretch = stretch;
        _bgs_vertical_stretch = stretch;
    }

    /**
     * @brief Sets the stretch of the mosaic applied to the backgrounds.
     * @param horizontal_stretch Horizontal stretch in the range [0..1].
     * @param vertical_stretch Vertical stretch in the range [0..1].
     */
    constexpr void set_bgs_stretch(fixed horizontal_stretch, fixed vertical_stretch)
    {
        BN_ASSERT(horizontal_stretch >= 0 && horizontal_stretch <= 1,
                   "Invalid horizontal stretch: ", horizontal_stretch);
        BN_ASSERT(vertical_stretch >= 0 && vertical_stretch <= 1, "Invalid vertical stretch: ", vertical_stretch);

        _bgs_horizontal_stretch = horizontal_stretch;
        _bgs_vertical_stretch = vertical_stretch;
    }

    /**
     * @brief Sets the horizontal stretch of the mosaic applied to both sprites and backgrounds.
     * @param horizontal_stretch Horizontal stretch in the range [0..1].
     */
    constexpr void set_horizontal_stretch(fixed horizontal_stretch)
    {
        BN_ASSERT(horizontal_stretch >= 0 && horizontal_stretch <= 1,
                   "Invalid horizontal stretch: ", horizontal_stretch);

        _sprites_horizontal_stretch = horizontal_stretch;
        _bgs_horizontal_stretch = horizontal_stretch;
    }

    /**
     * @brief Sets the vertical stretch of the mosaic applied to both sprites and backgrounds.
     * @param vertical_stretch Vertical stretch in the range [0..1].
     */
    constexpr void set_vertical_stretch(fixed vertical_stretch)
    {
        BN_ASSERT(vertical_stretch >= 0 && vertical_stretch <= 1, "Invalid vertical stretch: ", vertical_stretch);

        _sprites_vertical_stretch = vertical_stretch;
        _bgs_vertical_stretch = vertical_stretch;
    }

    /**
     * @brief Sets the stretch of the mosaic applied to both sprites and backgrounds.
     * @param stretch Stretch in the range [0..1].
     */
    constexpr void set_stretch(fixed stretch)
    {
        BN_ASSERT(stretch >= 0 && stretch <= 1, "Invalid stretch: ", stretch);

        _sprites_horizontal_stretch = stretch;
        _sprites_vertical_stretch = stretch;
        _bgs_horizontal_stretch = stretch;
        _bgs_vertical_stretch = stretch;
    }

    /**
     * @brief Sets the stretch of the mosaic applied to both sprites and backgrounds.
     * @param horizontal_stretch Horizontal stretch in the range [0..1].
     * @param vertical_stretch Vertical stretch in the range [0..1].
     */
    constexpr void set_stretch(fixed horizontal_stretch, fixed vertical_stretch)
    {
        BN_ASSERT(horizontal_stretch >= 0 && horizontal_stretch <= 1,
                   "Invalid horizontal stretch: ", horizontal_stretch);
        BN_ASSERT(vertical_stretch >= 0 && vertical_stretch <= 1, "Invalid vertical stretch: ", vertical_stretch);

        _sprites_horizontal_stretch = horizontal_stretch;
        _sprites_vertical_stretch = vertical_stretch;
        _bgs_horizontal_stretch = horizontal_stretch;
        _bgs_vertical_stretch = vertical_stretch;
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const mosaic_attributes& a, const mosaic_attributes& b) = default;

private:
    fixed _sprites_horizontal_stretch;
    fixed _sprites_vertical_stretch;
    fixed _bgs_horizontal_stretch;
    fixed _bgs_vertical_stretch;
};

}

#endif

