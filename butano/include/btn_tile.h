/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_TILE_H
#define BTN_TILE_H

/**
 * @file
 * btn::tile header file.
 *
 * @ingroup tile
 */

#include "btn_common.h"

namespace btn
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
