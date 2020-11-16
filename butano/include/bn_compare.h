/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_COMPARE_H
#define BTN_COMPARE_H

/**
 * @file
 * Standard library compare aliases header file.
 *
 * @ingroup std
 */

#include <compare>
#include "btn_common.h"

namespace btn
{
    using std::strong_ordering;

    using std::partial_ordering;

    using std::weak_ordering;
}

#endif
