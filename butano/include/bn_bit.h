/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BIT_H
#define BN_BIT_H

/**
 * @file
 * Standard library bit aliases header file.
 *
 * @ingroup std
 */

#include <bit>
#include "bn_common.h"

namespace bn
{
    using std::has_single_bit;

    using std::popcount;
}

#endif
