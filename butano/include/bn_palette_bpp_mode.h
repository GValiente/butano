/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_PALETTE_BPP_MODE_H
#define BN_PALETTE_BPP_MODE_H

/**
 * @file
 * bn::palette_bpp_mode header file.
 *
 * @ingroup palette
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Specifies the available bits per pixel modes.
 *
 * @ingroup palette
 */
enum class palette_bpp_mode
{
    BPP_4, //!< 4 bits per pixel (16 colors).
    BPP_8 //!< 8 bits per pixel (256 colors).
};

}

#endif

