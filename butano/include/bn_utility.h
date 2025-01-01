/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_UTILITY_H
#define BN_UTILITY_H

/**
 * @file
 * Standard library utility aliases header file.
 *
 * @ingroup utility
 */

#include <utility>
#include "bn_common.h"

namespace bn
{
    using std::move;

    using std::forward;

    using std::swap;

    using std::index_sequence;
    using std::make_index_sequence;

    using std::pair;
    using std::make_pair;
}

#endif
