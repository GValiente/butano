/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_DISPLAY_H
#define BTN_DISPLAY_H

#include "../hw/include/btn_hw_display_constants.h"

/**
 * @brief Display related functions.
 *
 * @ingroup display
 */
namespace btn::display
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
}

#endif
