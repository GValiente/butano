/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_ANY_FWD_H
#define BN_ANY_FWD_H

/**
 * @file
 * bn::iany and bn::any declaration header file.
 *
 * @ingroup any
 */

#include "bn_common.h"

namespace bn
{
    class iany;

    /**
     * @brief `std::any` like container with a fixed size buffer.
     *
     * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
     *
     * @tparam MaxSize Maximum size in bytes of the managed objects.
     * @tparam MaxAlignment Maximum alignment in bytes of the managed objects.
     *
     * @ingroup any
     */
    template<int MaxSize, int MaxAlignment = alignof(int)>
    class any;
}

#endif
