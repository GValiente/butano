/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_AFFINE_BG_BIG_MAP_CANVAS_SIZE_H
#define BN_AFFINE_BG_BIG_MAP_CANVAS_SIZE_H

/**
 * @file
 * bn::affine_bg_big_map_canvas_size header file.
 *
 * @ingroup bg_map
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Available affine background map canvas sizes enum.
 *
 * @ingroup bg_map
 */
enum class affine_bg_big_map_canvas_size : uint8_t
{
    NORMAL, //!< 256x256 pixels.
    BIG, //!< 512x512 pixels.
    HUGE //!< 1024x1024 pixels.
};

}

#endif
