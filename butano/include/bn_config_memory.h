/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
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
 * bn::malloc, bn::calloc, bn::realloc, bn::memory::ewram_alloc, bn::memory::ewram_calloc,
 * bn::memory::ewram_realloc and the `new` operator.
 *
 * @ingroup memory
 */
#ifndef BN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS
    #define BN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS 16
#endif

#endif
