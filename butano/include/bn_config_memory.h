/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_MEMORY_H
#define BN_CONFIG_MEMORY_H

/**
 * @file
 * Memory configuration header file.
 *
 * @ingroup memory
 */

#include "bn_common.h"

/**
 * @def BN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS
 *
 * Specifies the maximum number of memory blocks that can be allocated in EWRAM with
 * bn::malloc, bn::memory::ewram_alloc and the new operator.
 *
 * @ingroup memory
 */
#ifndef BN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS
    #define BN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS 16
#endif

#endif
