/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BACKDROP_H
#define BN_BACKDROP_H

/**
 * @file
 * bn::backdrop header file.
 *
 * @ingroup backdrop
 */

#include "bn_optional.h"

namespace bn
{
    class color;
}

/**
 * @brief Backdrop related functions.
 *
 * @ingroup backdrop
 */
namespace bn::backdrop
{
    /**
     * @brief Returns the overridden backdrop color if any, bn::nullopt otherwise.
     *
     * The backdrop color is the transparent color of the backgrounds.
     */
    [[nodiscard]] const optional<bn::color>& color();

    /**
     * @brief Sets the overridden backdrop color.
     *
     * The backdrop color is the transparent color of the backgrounds.
     */
    void set_color(bn::color color);

    /**
     * @brief Sets or removes the overridden backdrop color.
     *
     * The backdrop color is the transparent color of the backgrounds.
     */
    void set_color(const optional<bn::color>& color);

    /**
     * @brief Removes the overridden backdrop color, if any.
     *
     * The backdrop color is the transparent color of the backgrounds.
     */
    void remove_color();

    /**
     * @brief Indicates if the backdrop is part of the blending bottom layer or not.
     */
    [[nodiscard]] bool blending_bottom_enabled();

    /**
     * @brief Sets if the backdrop must be part of the blending bottom layer or not.
     */
    void set_blending_bottom_enabled(bool blending_bottom_enabled);
}

#endif
