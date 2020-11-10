/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_FIXED_FWD_H
#define BTN_FIXED_FWD_H

/**
 * @file
 * btn::fixed_t and btn::fixed declaration header file.
 *
 * @ingroup math
 */

#include "btn_common.h"

namespace btn
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
