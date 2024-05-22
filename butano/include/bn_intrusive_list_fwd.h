/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_INTRUSIVE_LIST_FWD_H
#define BN_INTRUSIVE_LIST_FWD_H

/**
 * @file
 * bn::intrusive_list declaration header file.
 *
 * @ingroup intrusive_list
 */

#include "bn_common.h"

namespace bn
{
    /**
     * @brief To be part of a bn::intrusive_list, values must inherit this class.
     *
     * @ingroup intrusive_list
     */
    class intrusive_list_node_type;

    /**
     * @brief `std::list` like container that doesn't contain values, it just references them.
     *
     * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
     *
     * @tparam Type Element type (it must inherit bn::intrusive_list_node_type class).
     *
     * @ingroup intrusive_list
     */
    template<class Type>
    class intrusive_list;
}

#endif
