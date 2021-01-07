/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_VECTOR_FWD_H
#define BN_VECTOR_FWD_H

/**
 * @file
 * bn::ivector and bn::vector declaration header file.
 *
 * @ingroup vector
 */

#include "bn_common.h"

namespace bn
{
    /**
     * @brief Base class of vector.
     *
     * Can be used as a reference type for all vector containers containing a specific type.
     *
     * @tparam Type Element type.
     *
     * @ingroup vector
     */
    template<typename Type>
    class ivector;

    /**
     * @brief Vector implementation that uses a fixed size buffer.
     *
     * @tparam Type Element type.
     * @tparam MaxSize Maximum number of elements that can be stored.
     *
     * @ingroup vector
     */
    template<typename Type, int MaxSize>
    class vector;
}

#endif
