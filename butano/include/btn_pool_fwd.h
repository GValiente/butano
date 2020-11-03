/*
 * Copyright (c) 2014 jwellbelove Embedded Template Library https://www.etlcpp.com
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_POOL_FWD_H
#define BTN_POOL_FWD_H

#include "btn_common.h"

namespace btn
{
    /**
     * @brief Base class of pool.
     *
     * Can be used as a reference type for all pool containers containing a specific type.
     *
     * @tparam Type Element type.
     *
     * @ingroup pool
     */
    template<typename Type>
    class ipool;

    /**
     * @brief Pool implementation that uses a fixed size buffer.
     *
     * @tparam Type Element type.
     * @tparam MaxSize Maximum number of elements that can be stored.
     *
     * @ingroup pool
     */
    template<typename Type, int MaxSize>
    class pool;
}

#endif
