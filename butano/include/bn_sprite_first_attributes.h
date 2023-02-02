/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_FIRST_ATTRIBUTES_H
#define BN_SPRITE_FIRST_ATTRIBUTES_H

/**
 * @file
 * bn::sprite_first_attributes header file.
 *
 * @ingroup sprite
 */

#include "bn_fixed.h"
#include "bn_assert.h"

namespace bn
{

/**
 * @brief Manages the attributes to commit to the first GBA register of a sprite.
 *
 * @ingroup sprite
 */
class sprite_first_attributes
{

public:
    constexpr sprite_first_attributes() = default;

    /**
     * @brief Constructor.
     * @param y Vertical position of a sprite (relative to its camera, if it has one).
     * @param mosaic_enabled Indicates if the mosaic effect is applied to a sprite or not.
     * @param blending_enabled Indicates if blending is applied to a sprite or not.
     * @param window_enabled Indicates if a sprite must be part of the silhouette of the sprite window or not.
     *
     * Keep in mind that blending and window attributes can't be enabled in a sprite at the same time.
     *
     * @param visible Indicates if a sprite must be committed to the GBA or not.
     */
    constexpr sprite_first_attributes(fixed y, bool mosaic_enabled, bool blending_enabled, bool window_enabled,
                                      bool visible) :
        _y(y),
        _mosaic_enabled(mosaic_enabled),
        _blending_enabled(blending_enabled),
        _window_enabled(window_enabled),
        _visible(visible)
    {
        BN_BASIC_ASSERT(! blending_enabled || ! window_enabled,
                        "Blending and window can't be enabled at the same time");
    }

    /**
     * @brief Returns the vertical position of a sprite (relative to its camera, if it has one).
     */
    [[nodiscard]] constexpr fixed y() const
    {
        return _y;
    }

    /**
     * @brief Sets the vertical position of a sprite (relative to its camera, if it has one).
     */
    void constexpr set_y(fixed y)
    {
        _y = y;
    }

    /**
     * @brief Indicates if the mosaic effect is applied to a sprite or not.
     */
    [[nodiscard]] constexpr bool mosaic_enabled() const
    {
        return _mosaic_enabled;
    }

    /**
     * @brief Sets if the mosaic effect is applied to a sprite or not.
     */
    constexpr void set_mosaic_enabled(bool mosaic_enabled)
    {
        _mosaic_enabled = mosaic_enabled;
    }

    /**
     * @brief Indicates if blending is applied to a sprite or not.
     */
    [[nodiscard]] constexpr bool blending_enabled() const
    {
        return _blending_enabled;
    }

    /**
     * @brief Sets if blending is applied to a sprite or not.
     *
     * Keep in mind that blending and window attributes can't be enabled in a sprite at the same time.
     */
    constexpr void set_blending_enabled(bool blending_enabled)
    {
        BN_BASIC_ASSERT(! blending_enabled || ! _window_enabled,
                        "Blending and window can't be enabled at the same time");

        _blending_enabled = blending_enabled;
    }

    /**
     * @brief Indicates if a sprite must be part of the silhouette of the sprite window or not.
     */
    [[nodiscard]] constexpr bool window_enabled() const
    {
        return _window_enabled;
    }

    /**
     * @brief Sets if a sprite must be part of the silhouette of the sprite window or not.
     *
     * Keep in mind that blending and window attributes can't be enabled in a sprite at the same time.
     */
    constexpr void set_window_enabled(bool window_enabled)
    {
        BN_BASIC_ASSERT(! _blending_enabled || ! window_enabled,
                        "Blending and window can't be enabled at the same time");

        _window_enabled = window_enabled;
    }

    /**
     * @brief Indicates if a sprite must be committed to the GBA or not.
     */
    [[nodiscard]] constexpr bool visible() const
    {
        return _visible;
    }

    /**
     * @brief Sets if a sprite must be committed to the GBA or not.
     */
    constexpr void set_visible(bool visible)
    {
        _visible = visible;
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const sprite_first_attributes& a,
                                                   const sprite_first_attributes& b) = default;

private:
    fixed _y = 0;
    bool _mosaic_enabled = false;
    bool _blending_enabled = false;
    bool _window_enabled = false;
    bool _visible = false;
};

}

#endif

