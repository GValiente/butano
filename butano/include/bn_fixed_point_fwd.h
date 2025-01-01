/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_FIXED_POINT_FWD_H
#define BN_FIXED_POINT_FWD_H

/**
 * @file
 * bn::fixed_point_t and bn::fixed_point declaration header file.
 *
 * @ingroup math
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Defines a two-dimensional point using fixed point precision.
 *
 * @ingroup math
 */
template<int Precision>
class fixed_point_t;

using fixed_point = fixed_point_t<12>; //!< Default precision fixed_point_t alias.

}

#endif
