/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_PALETTES_H
#define BN_SPRITE_PALETTES_H

/**
 * @file
 * bn::sprite_palettes header file.
 *
 * @ingroup sprite
 * @ingroup palette
 */

#include "bn_fixed.h"

namespace bn
{
    class color;
}

/**
 * @brief Sprite color palettes related functions.
 *
 * @ingroup sprite
 * @ingroup palette
 */
namespace bn::sprite_palettes
{
    /**
     * @brief Returns the number of used sprite colors.
     */
    [[nodiscard]] int used_colors_count();

    /**
     * @brief Returns the number of available sprite colors.
     */
    [[nodiscard]] int available_colors_count();

    /**
     * @brief Returns the brightness of all sprite color palettes.
     */
    [[nodiscard]] fixed brightness();

    /**
     * @brief Sets the brightness of all sprite color palettes.
     * @param brightness New brightness in the range [0..1].
     */
    void set_brightness(fixed brightness);

    /**
     * @brief Returns the contrast of all sprite color palettes.
     */
    [[nodiscard]] fixed contrast();

    /**
     * @brief Sets the contrast of all sprite color palettes.
     * @param contrast New contrast in the range [0..1].
     */
    void set_contrast(fixed contrast);

    /**
     * @brief Returns the intensity of all sprite color palettes.
     */
    [[nodiscard]] fixed intensity();

    /**
     * @brief Sets the intensity of all sprite color palettes.
     * @param intensity New intensity in the range [0..1].
     */
    void set_intensity(fixed intensity);

    /**
     * @brief Indicates if the colors of all sprite color palettes are inverted or not.
     */
    [[nodiscard]] bool inverted();

    /**
     * @brief Sets if the colors of all sprite color palettes must be inverted or not.
     */
    void set_inverted(bool inverted);

    /**
     * @brief Returns the intensity of the grayscale effect applied to all sprite color palettes.
     */
    [[nodiscard]] fixed grayscale_intensity();

    /**
     * @brief Sets the intensity of the grayscale effect applied to all sprite color palettes.
     * @param intensity New intensity in the range [0..1].
     */
    void set_grayscale_intensity(fixed intensity);

    /**
     * @brief Returns the intensity of the hue shift effect applied to all sprite color palettes.
     */
    [[nodiscard]] fixed hue_shift_intensity();

    /**
     * @brief Sets the intensity of the hue shift effect applied to all sprite color palettes.
     * @param intensity New intensity in the range [0..1].
     */
    void set_hue_shift_intensity(fixed intensity);

    /**
     * @brief Returns the color of the fade effect applied to all sprite color palettes.
     */
    [[nodiscard]] color fade_color();

    /**
     * @brief Returns the intensity of the fade effect applied to all sprite color palettes.
     */
    [[nodiscard]] fixed fade_intensity();

    /**
     * @brief Sets the color of the fade effect applied to all sprite color palettes.
     */
    void set_fade_color(color color);

    /**
     * @brief Sets the intensity of the fade effect applied to all sprite color palettes.
     * @param intensity New intensity in the range [0..1].
     */
    void set_fade_intensity(fixed intensity);

    /**
     * @brief Sets the color and the intensity of the fade effect applied to all sprite color palettes.
     * @param color New color.
     * @param intensity New intensity in the range [0..1].
     */
    void set_fade(color color, fixed intensity);

    /**
     * @brief Logs the current status of the sprite color palettes manager.
     */
    void log_status();
}

#endif
