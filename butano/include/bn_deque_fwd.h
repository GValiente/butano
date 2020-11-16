/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_DEQUE_FWD_H
#define BTN_DEQUE_FWD_H

/**
 * @file
 * btn::ideque and btn::deque declaration header file.
 *
 * @ingroup deque
 */

#include "btn_common.h"

namespace btn
{
    /**
     * @brief Base class of deque.
     *
     * Can be used as a reference type for all deque containers containing a specific type.
     *
     * @tparam Type Element type.
     *
     * @ingroup deque
     */
    template<typename Type>
    class ideque;

    /**
     * @brief Deque implementation that uses a fixed size buffer.
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
