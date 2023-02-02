/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_LIST_FWD_H
#define BN_LIST_FWD_H

/**
 * @file
 * bn::ilist and bn::list declaration header file.
 *
 * @ingroup list
 */

#include "bn_common.h"

namespace bn
{
    /**
     * @brief Base class of bn::list.
     *
     * Can be used as a reference type for all bn::list containers containing a specific type.
     *
     * @tparam Type Element type.
     *
     * @ingroup list
     */
    template<typename Type>
    class ilist;

    /**
     * @brief `std::list` like container with a fixed size buffer.
     *
     * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
     *
     * @tparam Type Element type.
     * @tparam MaxSize Maximum number of elements that can be stored.
     *
     * @ingroup list
     */
    template<typename Type, int MaxSize>
    class list;
}

#endif
