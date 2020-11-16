/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_FORWARD_LIST_FWD_H
#define BN_FORWARD_LIST_FWD_H

/**
 * @file
 * bn::iforward_list and bn::forward_list declaration header file.
 *
 * @ingroup forward_list
 */

#include "bn_common.h"

namespace bn
{
    /**
     * @brief Base class of forward_list.
     *
     * Can be used as a reference type for all forward_list containers containing a specific type.
     *
     * @tparam Type Element type.
     *
     * @ingroup forward_list
     */
    template<typename Type>
    class iforward_list;

    /**
     * @brief Forward list implementation that uses a fixed size buffer.
     *
     * @tparam Type Element type.
     * @tparam MaxSize Maximum number of elements that can be stored.
     *
     * @ingroup forward_list
     */
    template<typename Type, int MaxSize>
    class forward_list;
}

#endif
