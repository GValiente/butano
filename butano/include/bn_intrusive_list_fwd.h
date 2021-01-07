/*
 * Copyright (c) 2020-2021 Gustavo Valiente gustavo.valiente@protonmail.com
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
     * @brief To be part of a intrusive_list, values must inherit this class.
     *
     * @ingroup intrusive_list
     */
    class intrusive_list_node_type;

    /**
     * @brief Intrusive list implementation.
     *
     * @tparam Type Element type (it must inherit intrusive_list_node_type class).
     *
     * @ingroup intrusive_list
     */
    template<class Type>
    class intrusive_list;
}

#endif
