/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_ALGORITHM_H
#define BN_ALGORITHM_H

/**
 * @file
 * Standard library algorithm aliases header file.
 *
 * @ingroup std
 */

#include <algorithm>
#include "bn_common.h"

namespace bn
{
    using std::min;
    using std::max;
    using std::clamp;

    using std::distance;
    using std::advance;

    using std::equal;
    using std::lexicographical_compare;

    using std::fill;
    using std::fill_n;

    using std::copy;
    using std::copy_n;
    using std::copy_backward;

    using std::find;
    using std::find_if;

    using std::lower_bound;
    using std::upper_bound;

    using std::remove;
    using std::remove_if;

    using std::sort;
    using std::stable_sort;

    using std::reverse;

    using std::swap_ranges;
}

#endif
