/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 jwellbelove, https://github.com/ETLCPP/etl, http://www.etlcpp.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef BN_POOL_FWD_H
#define BN_POOL_FWD_H

/**
 * @file
 * bn::ipool and bn::pool declaration header file.
 *
 * @ingroup pool
 */

#include "bn_common.h"

namespace bn
{
    /**
     * @brief Base class of bn::pool.
     *
     * Can be used as a reference type for all pool containers containing a specific type.
     *
     * @tparam Type Element type.
     *
     * @ingroup pool
     */
    template<typename Type>
    class ipool;

    /**
     * @brief bn::ipool implementation that uses a fixed size buffer.
     *
     * @tparam Type Element type.
     * @tparam MaxSize Maximum number of elements that can be stored.
     *
     * @ingroup pool
     */
    template<typename Type, int MaxSize>
    class pool;
}

#endif
