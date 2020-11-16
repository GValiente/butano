/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_INTRUSIVE_FORWARD_LIST_FWD_H
#define BTN_INTRUSIVE_FORWARD_LIST_FWD_H

/**
 * @file
 * btn::intrusive_forward_list declaration header file.
 *
 * @ingroup intrusive_forward_list
 */

#include "btn_common.h"

namespace btn
{
    /**
     * @brief To be part of a intrusive_forward_list, values must inherit this class.
     *
     * @ingroup intrusive_forward_list
     */
    class intrusive_forward_list_node_type;

    /**
     * @brief Intrusive forward list implementation.
     *
     * @tparam Type Element type (it must inherit intrusive_forward_list_node_type class).
     *
     * @ingroup intrusive_forward_list
     */
    template<typename Type>
    class intrusive_forward_list;
}

#endif
