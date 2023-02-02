/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
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
     * @brief `std::array` like container.
     *
     * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
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
