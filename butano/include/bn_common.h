/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
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
#include "../hw/include/bn_hw_common.h"

/**
 * @def BN_UNREACHABLE
 *
 * If control flow reaches the point of this macro, the program is undefined.
 *
 * @ingroup other
 */
#define BN_UNREACHABLE __builtin_unreachable

#endif
