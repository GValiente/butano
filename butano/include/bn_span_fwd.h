/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_SPAN_FWD_H
#define BTN_SPAN_FWD_H

/**
 * @file
 * btn::span declaration header file.
 *
 * @ingroup span
 */

#include "btn_common.h"

namespace btn
{
    /**
     * @brief Refers to a contiguous sequence of elements with the first element of the sequence at position zero.
     *
     * The elements are not copied but referenced, so they should outlive the span to avoid dangling references.
     *
     * @tparam Type Element type.
     *
     * @ingroup span
     */
    template<typename Type>
    class span;
}

#endif
