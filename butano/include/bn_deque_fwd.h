/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_DEQUE_FWD_H
#define BN_DEQUE_FWD_H

/**
 * @file
 * bn::ideque and bn::deque declaration header file.
 *
 * @ingroup deque
 */

#include "bn_common.h"

namespace bn
{
    /**
     * @brief Base class of bn::deque.
     *
     * Can be used as a reference type for all bn::deque containers containing a specific type.
     *
     * @tparam Type Element type.
     *
     * @ingroup deque
     */
    template<typename Type>
    class ideque;

    /**
     * @brief A `std::deque` like container with a fixed size buffer.
     *
     * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
     *
     * @tparam Type Element type.
     * @tparam MaxSize Maximum number of elements that can be stored.
     *
     * @ingroup deque
     */
    template<typename Type, int MaxSize>
    class deque;
}

#endif
