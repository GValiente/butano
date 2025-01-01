/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_TILE_H
#define BN_TILE_H

/**
 * @file
 * bn::tile header file.
 *
 * @ingroup tile
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Represents a 8x8 block of 16 color pixels.
 *
 * @ingroup tile
 */
class tile
{

public:
    uint32_t data[8]; //!< Aligned tile data.
};

}

#endif
