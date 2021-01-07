/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_ARRAY_FWD_H
#define BN_ARRAY_FWD_H

/**
 * @file
 * bn::array declaration header file.
 *
 * @ingroup array
 */

#include "bn_common.h"

namespace bn
{
    /**
     * @brief Container that encapsulates fixed size arrays.
     *
     * @tparam Type Element type.
     * @tparam Size Number of stored elements.
     *
     * @ingroup array
     */
    template<typename Type, int Size>
    class array;
}

#endif
