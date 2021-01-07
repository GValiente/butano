/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
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
    constexpr const color black(0x0000); //!< Black color.
    constexpr const color gray(0x4210); //!< Gray color.
    constexpr const color white(0x7FFF); //!< White color.

    constexpr const color red(0x001F); //!< Red color.
    constexpr const color green(0x0200); //!< Green color.
    constexpr const color blue(0x7C00); //!< Blue color.

    constexpr const color yellow(0x03FF); //!< Yellow color.
    constexpr const color orange(0x021F); //!< Orange color.
    constexpr const color maroon(0x0010); //!< Maroon color.
    constexpr const color purple(0x4010); //!< Purple color.

    constexpr const color lime(0x03E0); //!< Lime color.
    constexpr const color magenta(0x7C1F); //!< Magenta color.
    constexpr const color cyan(0x7FE0); //!< Cyan color.
    constexpr const color olive(0x0210); //!< Olive color.
    constexpr const color navy(0x4000); //!< Navy color.
    constexpr const color teal(0x4200); //!< Teal color.
    constexpr const color silver(0x6318); //!< Silver color.
    constexpr const color fuchsia(0x7C1F); //!< Fuchsia color.
    constexpr const color cream(0x7BFF); //!< Cream color.
}

#endif
