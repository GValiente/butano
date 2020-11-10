/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_ANY_FWD_H
#define BTN_ANY_FWD_H

/**
 * @file
 * btn::iany and btn::any declaration header file.
 *
 * @ingroup any
 */

#include "btn_common.h"

namespace btn
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
