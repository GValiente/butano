/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_INTRUSIVE_FORWARD_LIST_FWD_H
#define BN_INTRUSIVE_FORWARD_LIST_FWD_H

/**
 * @file
 * bn::intrusive_forward_list declaration header file.
 *
 * @ingroup intrusive_forward_list
 */

#include "bn_common.h"

namespace bn
{
    /**
     * @brief To be part of a bn::intrusive_forward_list, values must inherit this class.
     *
     * @ingroup intrusive_forward_list
     */
    class intrusive_forward_list_node_type;

    /**
     * @brief `std::forward_list` like container that doesn't contain values, it just references them.
     *
     * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
     *
     * @tparam Type Element type (it must inherit bn::intrusive_forward_list_node_type class).
     *
     * @ingroup intrusive_forward_list
     */
    template<typename Type>
    class intrusive_forward_list;
}

#endif
