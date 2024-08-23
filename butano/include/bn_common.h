/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_COMMON_H
#define BN_COMMON_H

/**
 * @file
 * Common header file.
 *
 * @ingroup other
 */

#include <cstdint>
#include <cstddef>
#include "../hw/include/bn_hw_common.h"

/**
 * @def BN_CONST
 *
 * The return value of a function with this attribute is solely a function of the arguments.
 *
 * @ingroup other
 */
#define BN_CONST __attribute__((const))

/// @cond DO_NOT_DOCUMENT

#define BN_UNREACHABLE __builtin_unreachable

/// @endcond

namespace bn
{
    using std::nullptr_t;
}

#endif
