/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
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
    /**
     * @brief Base class of any.
     *
     * Can be used as a reference type for all any containers.
     *
     * @ingroup any
     */
    class iany;

    /**
     * @brief Any implementation that uses a fixed size buffer.
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
