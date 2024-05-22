/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_FIXED_FWD_H
#define BN_FIXED_FWD_H

/**
 * @file
 * bn::fixed_t and bn::fixed declaration header file.
 *
 * @ingroup math
 */

#include "bn_common.h"

namespace bn
{

/**
 * @brief Poor man's float.
 *
 * It fakes numbers with decimal points with integers.
 *
 * @tparam Precision Number of bits used for the fractional part.
 *
 * @ingroup math
 */
template<int Precision>
class fixed_t;

using fixed = fixed_t<12>; //!< Default precision fixed_t alias.

}

#endif
