/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CONFIG_BEST_FIT_ALLOCATOR_H
#define BN_CONFIG_BEST_FIT_ALLOCATOR_H

/**
 * @file
 * bn::best_fit_allocator configuration header file.
 *
 * @ingroup allocator
 */

#include "bn_common.h"

/**
 * @def BN_CFG_BEST_FIT_ALLOCATOR_SANITY_CHECK_ENABLED
 *
 * Specifies if bn::best_fit_allocator sanity check is enabled or not.
 *
 * Sanity check asserts if the internal state of the allocator is valid.
 *
 * @ingroup allocator
 */
#ifndef BN_CFG_BEST_FIT_ALLOCATOR_SANITY_CHECK_ENABLED
    #define BN_CFG_BEST_FIT_ALLOCATOR_SANITY_CHECK_ENABLED false
#endif

/**
 * @def BN_CFG_BEST_FIT_ALLOCATOR_FREE_CHECK_ENABLED
 *
 * Specifies if bn::best_fit_allocator::free checks if the input pointer is valid or not.
 *
 * @ingroup allocator
 */
#ifndef BN_CFG_BEST_FIT_ALLOCATOR_FREE_CHECK_ENABLED
    #define BN_CFG_BEST_FIT_ALLOCATOR_FREE_CHECK_ENABLED false
#endif

#endif
