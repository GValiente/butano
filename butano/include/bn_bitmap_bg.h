/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BITMAP_BG_H
#define BN_BITMAP_BG_H

/**
 * @file
 * bn::bitmap_bg header file.
 *
 * @ingroup bitmap_bg
 */

#include "bn_size.h"
#include "../hw/include/bn_hw_bitmap_bg_constants.h"

/**
 * @brief Bitmap background related functions.
 *
 * @ingroup bitmap_bg
 */
namespace bn::bitmap_bg
{
    /**
     * @brief Returns the width of palette bitmap backgrounds in pixels (240).
     */
    [[nodiscard]] constexpr int palette_width()
    {
        return hw::bitmap_bg::palette_width();
    }

    /**
     * @brief Returns the height of palette bitmap backgrounds in pixels (160).
     */
    [[nodiscard]] constexpr int palette_height()
    {
        return hw::bitmap_bg::palette_height();
    }

    /**
     * @brief Returns the size of palette bitmap backgrounds in pixels (240x160).
     */
    [[nodiscard]] constexpr size palette_size()
    {
        return bn::size(palette_width(), palette_height());
    }

    /**
     * @brief Returns the width of dual page direct bitmap backgrounds in pixels (240).
     */
    [[nodiscard]] constexpr int dp_direct_width()
    {
        return hw::bitmap_bg::dp_direct_width();
    }

    /**
     * @brief Returns the height of dual page direct bitmap backgrounds in pixels (160).
     */
    [[nodiscard]] constexpr int dp_direct_height()
    {
        return hw::bitmap_bg::dp_direct_height();
    }

    /**
     * @brief Returns the size of dual page direct bitmap backgrounds in pixels (240x160).
     */
    [[nodiscard]] constexpr size dp_direct_size()
    {
        return bn::size(dp_direct_width(), dp_direct_height());
    }
}

#endif
