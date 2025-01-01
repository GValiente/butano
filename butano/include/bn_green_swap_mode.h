/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_GREEN_SWAP_MODE_H
#define BN_GREEN_SWAP_MODE_H

/**
 * @file
 * bn::green_swap_mode header file.
 *
 * @ingroup green_swap
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Specifies how a layer must be displayed when green swap is enabled.
 *
 * @ingroup green_swap
 */
enum class green_swap_mode : uint8_t
{
    DEFAULT, //!< The default green swap dirt effect.
    HALF_TRANSPARENT_A, //!< Half of the columns are hidden.
    HALF_TRANSPARENT_B, //!< The other half of the columns are hidden.
    DUPLICATED //!< Pixels are duplicated
};

}

#endif
