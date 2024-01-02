/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_TOP_LEFT_FIXED_RECT_FWD_H
#define BN_TOP_LEFT_FIXED_RECT_FWD_H

/**
 * @file
 * bn::top_left_fixed_rect_t and bn::top_left_fixed_rect declaration header file.
 *
 * @ingroup math
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Defines a two-dimensional rectangle using fixed point precision and a top-left point as position.
 *
 * @ingroup math
 */
template<int Precision>
class top_left_fixed_rect_t;

using top_left_fixed_rect = top_left_fixed_rect_t<12>; //!< Default precision top_left_fixed_rect_t alias.

}

#endif
