/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_SHAPE_SIZE_H
#define BN_SPRITE_SHAPE_SIZE_H

/**
 * @file
 * bn::sprite_shape header file.
 *
 * @ingroup sprite
 */

#include "bn_assert.h"
#include "bn_bpp_mode.h"

namespace bn
{

/**
 * @brief Valid sprite shapes enum.
 *
 * @ingroup sprite
 */
enum class sprite_shape : uint8_t
{
    SQUARE,
    WIDE,
    TALL
};


/**
 * @brief Valid sprite sizes enum.
 *
 * @ingroup sprite
 */
enum class sprite_size : uint8_t
{
    SMALL,
    NORMAL,
    BIG,
    HUGE
};


/**
 * @brief Calculates sprite dimensions from its shape and size.
 *
 * <table>
 * <caption> Available sprite pixel sizes </caption>
 * <tr><th> shape / size    <th> small  <th> normal <th> big    <th> huge
 * <tr><th> square          <td> 8x8    <td> 16x16  <td> 32x32  <td> 64x64
 * <tr><th> wide            <td> 16x8   <td> 32x8   <td> 32x16  <td> 64x32
 * <tr><th> tall            <td> 8x16   <td> 8x32   <td> 16x32  <td> 32x64
 * </table>
 *
 * @ingroup sprite
 */
class sprite_shape_size
{

public:
    /**
     * @brief Constructor.
     * @param shape sprite_shape enum.
     * @param size sprite_size enum.
     */
    constexpr sprite_shape_size(sprite_shape shape, sprite_size size) :
        _shape(shape),
        _size(size)
    {
    }

    /**
     * @brief Returns the sprite_shape enum.
     */
    [[nodiscard]] constexpr sprite_shape shape() const
    {
        return _shape;
    }

    /**
     * @brief Returns the sprite_size enum.
     */
    [[nodiscard]] constexpr sprite_size size() const
    {
        return _size;
    }

    /**
     * @brief Returns the width of the sprite in pixels.
     */
    [[nodiscard]] constexpr int width() const
    {
        switch(_shape)
        {

        case sprite_shape::SQUARE:
            switch(_size)
            {

            case sprite_size::SMALL:
                return 8;

            case sprite_size::NORMAL:
                return 16;

            case sprite_size::BIG:
                return 32;

            case sprite_size::HUGE:
                return 64;

            default:
                BN_ERROR("Invalid size: ", int(_size));
                return 0;
            }

        case sprite_shape::WIDE:
            switch(_size)
            {

            case sprite_size::SMALL:
                return 16;

            case sprite_size::NORMAL:
                return 32;

            case sprite_size::BIG:
                return 32;

            case sprite_size::HUGE:
                return 64;

            default:
                BN_ERROR("Invalid size: ", int(_size));
                return 0;
            }

        case sprite_shape::TALL:
            switch(_size)
            {

            case sprite_size::SMALL:
                return 8;

            case sprite_size::NORMAL:
                return 8;

            case sprite_size::BIG:
                return 16;

            case sprite_size::HUGE:
                return 32;

            default:
                BN_ERROR("Invalid size: ", int(_size));
                return 0;
            }

        default:
            BN_ERROR("Invalid shape: ", int(_shape));
            return 0;
        }
    }

    /**
     * @brief Returns the height of the sprite in pixels.
     */
    [[nodiscard]] constexpr int height() const
    {
        switch(_shape)
        {

        case sprite_shape::SQUARE:
            switch(_size)
            {

            case sprite_size::SMALL:
                return 8;

            case sprite_size::NORMAL:
                return 16;

            case sprite_size::BIG:
                return 32;

            case sprite_size::HUGE:
                return 64;

            default:
                BN_ERROR("Invalid size: ", int(_size));
                return 0;
            }

        case sprite_shape::WIDE:
            switch(_size)
            {

            case sprite_size::SMALL:
                return 8;

            case sprite_size::NORMAL:
                return 8;

            case sprite_size::BIG:
                return 16;

            case sprite_size::HUGE:
                return 32;

            default:
                BN_ERROR("Invalid size: ", int(_size));
                return 0;
            }

        case sprite_shape::TALL:
            switch(_size)
            {

            case sprite_size::SMALL:
                return 16;

            case sprite_size::NORMAL:
                return 32;

            case sprite_size::BIG:
                return 32;

            case sprite_size::HUGE:
                return 64;

            default:
                BN_ERROR("Invalid size: ", int(_size));
                return 0;
            }

        default:
            BN_ERROR("Invalid shape: ", int(_shape));
            return 0;
        }
    }

    /**
     * @brief Returns the area of the sprite in pixels.
     */
    [[nodiscard]] constexpr int pixels_count() const
    {
        return width() * height();
    }

    /**
     * @brief Returns the area of the sprite in tiles.
     * @param bpp Tiles' bits per pixel mode.
     * @return Area of the sprite in tiles.
     */
    [[nodiscard]] constexpr int tiles_count(bpp_mode bpp) const
    {
        return bpp == bpp_mode::BPP_8 ? pixels_count() / 32 : pixels_count() / 64;
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const sprite_shape_size& a, const sprite_shape_size& b) = default;

private:
    sprite_shape _shape;
    sprite_size _size;
};

}

#endif
