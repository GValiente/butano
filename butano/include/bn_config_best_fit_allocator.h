/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
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
 * @def BN_CFG_BEST_FIT_ALLOCATOR_SANITY_CHECK
 *
 * Specifies if bn::best_fit_allocator sanity check is enabled or not.
 *
 * @ingroup allocator
 */
#ifndef BN_CFG_BEST_FIT_ALLOCATOR_SANITY_CHECK
    #define BN_CFG_BEST_FIT_ALLOCATOR_SANITY_CHECK false
#endif

#endif
