/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPAN_FWD_H
#define BN_SPAN_FWD_H

/**
 * @file
 * bn::span declaration header file.
 *
 * @ingroup span
 */

#include "bn_common.h"

namespace bn
{
    /**
     * @brief `std::span` like container.
     *
     * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
     *
     * @tparam Type Element type.
     *
     * @ingroup span
     */
    template<typename Type>
    class span;
}

#endif
