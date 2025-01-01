/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_FIXED_SIZE_FWD_H
#define BN_FIXED_SIZE_FWD_H

/**
 * @file
 * bn::fixed_size_t and bn::fixed_size declaration header file.
 *
 * @ingroup math
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Defines the size of a two-dimensional object using fixed point precision.
 *
 * @ingroup math
 */
template<int Precision>
class fixed_size_t;

using fixed_size = fixed_size_t<12>; //!< Default precision fixed_size_t alias.

}

#endif
