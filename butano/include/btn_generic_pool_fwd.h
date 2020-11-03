/*
 * Copyright (c) 2014 jwellbelove Embedded Template Library https://www.etlcpp.com
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_GENERIC_POOL_FWD_H
#define BTN_GENERIC_POOL_FWD_H

#include "btn_common.h"

namespace btn
{
    /**
     * @brief Base class of generic_pool.
     *
     * Can be used as a reference type for all generic_pool containers containing a specific maximum size.
     *
     * @tparam MaxElementSize Maximum element size in bytes.
     *
     * @ingroup pool
     */
    template<int MaxElementSize>
    class igeneric_pool;

    /**
     * @brief Generic pool implementation that uses a fixed size buffer.
     *
     * @tparam MaxElementSize Element size.
     * @tparam MaxSize Maximum number of elements that can be stored.
     *
     * @ingroup pool
     */
    template<int MaxElementSize, int MaxSize>
    class generic_pool;
}

#endif
