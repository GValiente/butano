/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_COMMON_H
#define BTN_COMMON_H

/**
 * @file
 * Common header file.
 *
 * @ingroup other
 */

#include <cstdint>
#include "../hw/include/btn_hw_common.h"

/**
 * @def BTN_UNREACHABLE
 *
 * If control flow reaches the point of this macro, the program is undefined.
 *
 * @ingroup other
 */
#define BTN_UNREACHABLE __builtin_unreachable

#endif
