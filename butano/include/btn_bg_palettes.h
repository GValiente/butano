/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_BG_PALETTES_H
#define BTN_BG_PALETTES_H

/**
 * @file
 * btn::bg_palettes header file.
 *
 * @ingroup bg
 * @ingroup palette
 */

#include "btn_fixed_fwd.h"
#include "btn_optional_fwd.h"

namespace btn
{
    class color;
}

/**
 * @brief Background color palettes related functions.
 *
 * @ingroup bg
 * @ingroup palette
 */
namespace btn::bg_palettes
{
    /**
     * @brief Returns the number of used background colors.
     */
    [[nodiscard]] int used_colors_count();

    /**
     * @brief Returns the number of available background colors.
     */
    [[nodiscard]] int available_colors_count();

    /**
     * @brief Returns the overridden transparent color of the backgrounds if any, `nullopt` otherwise.
     */
    [[nodiscard]] const optional<color>& transparent_color();

    /**
     * @brief Sets the overridden transparent color of the backgrounds.
     */
    void set_transparent_color(color transparent_color);

    /**
     * @brief Removes the overridden transparent color of the backgrounds, if any.
     */
    void remove_transparent_color();

    /**
     * @brief Returns the brightness of all background color palettes.
     */
    [[nodiscard]] fixed brightness();

    /**
     * @brief Sets the brightness of all background color palettes.
     * @param brightness New brightness in the range [0..1].
     */
    void set_brightness(fixed brightness);

    /**
     * @brief Returns the contrast of all background color palettes.
     */
    [[nodiscard]] fixed contrast();

    /**
     * @brief Sets the contrast of all background color palettes.
     * @param contrast New contrast in the range [0..1].
     */
    void set_contrast(fixed contrast);

    /**
     * @brief Returns the intensity of all background color palettes.
     */
    [[nodiscard]] fixed intensity();

    /**
     * @brief Sets the intensity of all background color palettes.
     * @param intensity New intensity in the range [0..1].
     */
    void set_intensity(fixed intensity);

    /**
     * @brief Indicates if the colors of all background color palettes are inverted or not.
     */
    [[nodiscard]] bool inverted();

    /**
     * @brief Sets if the colors of all background color palettes must be inverted or not.
     */
    void set_inverted(bool inverted);

    /**
     * @brief Returns the intensity of the grayscale effect applied to all background color palettes.
     */
    [[nodiscard]] fixed grayscale_intensity();

    /**
     * @brief Sets the intensity of the grayscale effect applied to all background color palettes.
     * @param intensity New intensity in the range [0..1].
     */
    void set_grayscale_intensity(fixed intensity);

    /**
     * @brief Returns the color of the fade effect applied to all background color palettes.
     */
    [[nodiscard]] color fade_color();

    /**
     * @brief Returns the intensity of the fade effect applied to all background color palettes.
     */
    [[nodiscard]] fixed fade_intensity();

    /**
     * @brief Sets the color of the fade effect applied to all background color palettes.
     */
    void set_fade_color(color color);

    /**
     * @brief Sets the intensity of the fade effect applied to all background color palettes.
     * @param intensity New intensity in the range [0..1].
     */
    void set_fade_intensity(fixed intensity);

    /**
     * @brief Sets the color and the intensity of the fade effect applied to all background color palettes.
     * @param color New color.
     * @param intensity New intensity in the range [0..1].
     */
    void set_fade(color color, fixed intensity);
}

#endif
