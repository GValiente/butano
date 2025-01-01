/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_COLORS_H
#define BN_COLORS_H

/**
 * @file
 * bn::colors header file.
 *
 * @ingroup color
 */

#include "bn_color.h"

/**
 * @brief Some constant colors from TONC.
 *
 * @ingroup color
 */
namespace bn::colors
{
    constexpr color black(0x0000); //!< Black color.
    constexpr color gray(0x4210); //!< Gray color.
    constexpr color white(0x7FFF); //!< White color.

    constexpr color red(0x001F); //!< Red color.
    constexpr color green(0x0200); //!< Green color.
    constexpr color blue(0x7C00); //!< Blue color.

    constexpr color yellow(0x03FF); //!< Yellow color.
    constexpr color orange(0x021F); //!< Orange color.
    constexpr color maroon(0x0010); //!< Maroon color.
    constexpr color purple(0x4010); //!< Purple color.

    constexpr color lime(0x03E0); //!< Lime color.
    constexpr color magenta(0x7C1F); //!< Magenta color.
    constexpr color cyan(0x7FE0); //!< Cyan color.
    constexpr color olive(0x0210); //!< Olive color.
    constexpr color navy(0x4000); //!< Navy color.
    constexpr color teal(0x4200); //!< Teal color.
    constexpr color silver(0x6318); //!< Silver color.
    constexpr color fuchsia(0x7C1F); //!< Fuchsia color.
    constexpr color cream(0x7BFF); //!< Cream color.
}

#endif
