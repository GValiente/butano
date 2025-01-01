/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BPP_MODE_H
#define BN_BPP_MODE_H

/**
 * @file
 * bn::bpp_mode header file.
 *
 * @ingroup color
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Specifies the available bits per pixel modes.
 *
 * @ingroup color
 */
enum class bpp_mode : uint8_t
{
    BPP_4, //!< 4 bits per pixel (16 colors).
    BPP_8 //!< 8 bits per pixel (256 colors).
};

}

#endif

