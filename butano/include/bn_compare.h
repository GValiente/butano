/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_COMPARE_H
#define BN_COMPARE_H

/**
 * @file
 * Standard library compare aliases header file.
 *
 * @ingroup std
 */

#include <compare>
#include "bn_common.h"

namespace bn
{
    using std::strong_ordering;

    using std::partial_ordering;

    using std::weak_ordering;
}

#endif
