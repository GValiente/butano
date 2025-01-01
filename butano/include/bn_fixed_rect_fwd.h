/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_FIXED_RECT_FWD_H
#define BN_FIXED_RECT_FWD_H

/**
 * @file
 * bn::fixed_rect_t and bn::fixed_rect declaration header file.
 *
 * @ingroup math
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Defines a two-dimensional rectangle using fixed point precision and a center point as position.
 *
 * @ingroup math
 */
template<int Precision>
class fixed_rect_t;

using fixed_rect = fixed_rect_t<12>; //!< Default precision fixed_rect_t alias.

}

#endif
