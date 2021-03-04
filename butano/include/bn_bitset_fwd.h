/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_BITSET_FWD_H
#define BN_BITSET_FWD_H

/**
 * @file
 * bn::bitset declaration header file.
 *
 * @ingroup bitset
 */

#include "bn_common.h"

namespace bn
{
    /**
     * @brief bitset implementation.
     *
     * @tparam Size Number of bits stored.
     *
     * @ingroup bitset
     */
    template<int Size>
    class bitset;
}

#endif
