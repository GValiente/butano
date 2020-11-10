/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_CONFIG_MEMORY_H
#define BTN_CONFIG_MEMORY_H

/**
 * @file
 * Memory configuration header file.
 *
 * @ingroup memory
 */

#include "btn_common.h"

/**
 * @def BTN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS
 *
 * Specifies the maximum number of memory blocks that can be allocated in EWRAM with
 * btn::malloc, btn::memory::ewram_alloc and the new operator.
 *
 * @ingroup memory
 */
#ifndef BTN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS
    #define BTN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS 16
#endif

#endif
