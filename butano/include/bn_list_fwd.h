/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_LIST_FWD_H
#define BTN_LIST_FWD_H

/**
 * @file
 * btn::ilist and btn::list declaration header file.
 *
 * @ingroup list
 */

#include "btn_common.h"

namespace btn
{
    /**
     * @brief Base class of list.
     *
     * Can be used as a reference type for all list containers containing a specific type.
     *
     * @tparam Type Element type.
     *
     * @ingroup list
     */
    template<typename Type>
    class ilist;

    /**
     * @brief List implementation that uses a fixed size buffer.
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
