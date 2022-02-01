/*
 * Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BEST_FIT_ALLOCATOR_FWD_H
#define BN_BEST_FIT_ALLOCATOR_FWD_H

/**
 * @file
 * bn::ibest_fit_allocator and bn::best_fit_allocator declaration header file.
 *
 * @ingroup allocator
 */

#include "bn_common.h"

namespace bn
{
    class ibest_fit_allocator;

    /**
     * @brief ibest_fit_allocator implementation that uses a fixed number of maximum items.
     *
     * @tparam MaxItems Maximum number of items that can be stored.
     *
     * @ingroup allocator
     */
    template<int MaxItems>
    class best_fit_allocator;
}

#endif
