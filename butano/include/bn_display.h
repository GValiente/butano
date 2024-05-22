/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DISPLAY_H
#define BN_DISPLAY_H

/**
 * @file
 * bn::display header file.
 *
 * @ingroup display
 */

#include "bn_size.h"
#include "../hw/include/bn_hw_display_constants.h"

/**
 * @brief Display related functions.
 *
 * @ingroup display
 */
namespace bn::display
{
    /**
     * @brief Returns the screen width in pixels (240).
     */
    [[nodiscard]] constexpr int width()
    {
        return hw::display::width();
    }

    /**
     * @brief Returns the screen height in pixels (160).
     */
    [[nodiscard]] constexpr int height()
    {
        return hw::display::height();
    }

    /**
     * @brief Returns the screen size in pixels (240x160).
     */
    [[nodiscard]] constexpr size size()
    {
        return bn::size(width(), height());
    }
}

#endif
