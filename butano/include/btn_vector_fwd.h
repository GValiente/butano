/*
 * Copyright (c) 2014 jwellbelove Embedded Template Library https://www.etlcpp.com
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_VECTOR_FWD_H
#define BTN_VECTOR_FWD_H

#include "btn_common.h"

/**
 * @defgroup vector vector
 *
 * A vector with the capacity defined at compile time.
 */

namespace btn
{
    /**
     * @brief Base class for vectors.
     *
     * Can be used as a reference type for all vectors containing a specific type.
     *
     * STL equivalent: std::vector
     *
     * @tparam Type The element type.
     *
     * @ingroup vector
     */
    template<typename Type>
    class ivector;

    /**
     * @brief Vector implementation that uses a fixed size buffer.
     *
     * STL equivalent: std::vector
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
