/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_COLOR_EFFECT_H
#define BN_COLOR_EFFECT_H

/**
 * @file
 * bn::color_effect header file.
 *
 * @ingroup color
 */

#include "bn_span.h"
#include "bn_fixed.h"

namespace bn
{
    class color;
}

/**
 * @brief Color effects related functions.
 *
 * @ingroup color
 */
namespace bn::color_effect
{

/**
 * @brief Increases the brightness of all referenced colors.
 * @param brightness Brightness amount in the range [0..1].
 * @param colors_ref Colors reference.
 */
void brightness(fixed brightness, span<color> colors_ref);

/**
 * @brief Increases the brightness of all colors referenced by source_colors_ref,
 * storing the result in destination_colors_ref.
 * @param source_colors_ref Source colors reference.
 * @param brightness Brightness amount in the range [0..1].
 * @param destination_colors_ref Destination colors reference.
 */
void brightness(const span<const color>& source_colors_ref, fixed brightness, span<color> destination_colors_ref);

/**
 * @brief Increases the contrast of all referenced colors.
 * @param contrast Contrast amount in the range [0..1].
 * @param colors_ref Colors reference.
 */
void contrast(fixed contrast, span<color> colors_ref);

/**
 * @brief Increases the contrast of all colors referenced by source_colors_ref,
 * storing the result in destination_colors_ref.
 * @param source_colors_ref Source colors reference.
 * @param contrast Contrast amount in the range [0..1].
 * @param destination_colors_ref Destination colors reference.
 */
void contrast(const span<const color>& source_colors_ref, fixed contrast, span<color> destination_colors_ref);

/**
 * @brief Increases the intensity of all referenced colors.
 * @param intensity Intensity amount in the range [0..1].
 * @param colors_ref Colors reference.
 */
void intensity(fixed intensity, span<color> colors_ref);

/**
 * @brief Increases the intensity of all colors referenced by source_colors_ref,
 * storing the result in destination_colors_ref.
 * @param source_colors_ref Source colors reference.
 * @param intensity Intensity amount in the range [0..1].
 * @param destination_colors_ref Destination colors reference.
 */
void intensity(const span<const color>& source_colors_ref, fixed intensity, span<color> destination_colors_ref);

/**
 * @brief Inverts all referenced colors.
 * @param colors_ref Colors reference.
 */
void invert(span<color> colors_ref);

/**
 * @brief Inverts all colors referenced by source_colors_ref, storing the result in destination_colors_ref.
 * @param source_colors_ref Source colors reference.
 * @param destination_colors_ref Destination colors reference.
 */
void invert(const span<const color>& source_colors_ref, span<color> destination_colors_ref);

/**
 * @brief Applies a grayscale effect to all referenced colors.
 * @param intensity Grayscale effect intensity in the range [0..1].
 * @param colors_ref Colors reference.
 * It must be 4-byte aligned and have an even number of colors.
 */
void grayscale(fixed intensity, span<color> colors_ref);

/**
 * @brief Applies a grayscale effect to all colors referenced by source_colors_ref,
 * storing the result in destination_colors_ref.
 * @param source_colors_ref Source colors reference.
 * It must be 4-byte aligned and have an even number of colors.
 * @param intensity Grayscale effect intensity in the range [0..1].
 * @param destination_colors_ref Destination colors reference.
 * It must be 4-byte aligned and have an even number of colors.
 */
void grayscale(const span<const color>& source_colors_ref, fixed intensity, span<color> destination_colors_ref);

/**
 * @brief Applies a hue shift effect to all referenced colors.
 * @param intensity Hue shift effect intensity in the range [0..1].
 * @param colors_ref Colors reference.
 */
void hue_shift(fixed intensity, span<color> colors_ref);

/**
 * @brief Applies a hue shift effect to all colors referenced by source_colors_ref,
 * storing the result in destination_colors_ref.
 * @param source_colors_ref Source colors reference.
 * @param intensity Hue shift effect intensity in the range [0..1].
 * @param destination_colors_ref Destination colors reference.
 */
void hue_shift(const span<const color>& source_colors_ref, fixed intensity, span<color> destination_colors_ref);

/**
 * @brief Blends two color arrays referenced by first_source_colors_ref and second_source_colors_ref,
 * storing the result in destination_colors_ref.
 * @param first_source_colors_ref First source colors reference.
 * It must be 4-byte aligned and have an even number of colors.
 * @param second_source_colors_ref Second source colors reference.
 * It must be 4-byte aligned and have an even number of colors.
 * @param weight Blend weight in the range [0..1].
 * @param destination_colors_ref Destination colors reference.
 * It must be 4-byte aligned and have an even number of colors.
 */
void blend(const span<const color>& first_source_colors_ref, const span<const color>& second_source_colors_ref,
           fixed weight, span<color> destination_colors_ref);

/**
 * @brief Applies a fade effect to all referenced colors.
 * @param fade_color Fade effect color.
 * @param intensity Fade effect intensity in the range [0..1].
 * @param colors_ref Colors reference.
 * It must be 4-byte aligned and have an even number of colors.
 */
void fade(color fade_color, fixed intensity, span<color> colors_ref);

/**
 * @brief Applies a fade effect to all colors referenced by source_colors_ref,
 * storing the result in destination_colors_ref.
 * @param source_colors_ref Source colors reference.
 * It must be 4-byte aligned and have an even number of colors.
 * @param fade_color Fade effect color.
 * @param intensity Fade effect intensity in the range [0..1].
 * @param destination_colors_ref Destination colors reference.
 * It must be 4-byte aligned and have an even number of colors.
 */
void fade(const span<const color>& source_colors_ref, color fade_color, fixed intensity,
          span<color> destination_colors_ref);

/**
 * @brief Rotates all referenced colors to the right.
 * @param rotate_count Number of colors to rotate to the right
 * in the range [1 - colors_count .. colors_count - 1].
 * @param colors_ref Colors reference.
 */
void rotate(int rotate_count, span<color> colors_ref);

/**
 * @brief Rotates all colors referenced by source_colors_ref to the right,
 * storing the result in destination_colors_ref.
 *
 * If the source and destination tiles, colors or map cells overlap, the behavior is undefined.
 *
 * @param source_colors_ref Source colors reference.
 * @param rotate_count Number of colors to rotate to the right
 * in the range [1 - colors_count .. colors_count - 1].
 * @param destination_colors_ref Destination colors reference.
 */
void rotate(const span<const color>& source_colors_ref, int rotate_count, span<color> destination_colors_ref);

}

#endif
