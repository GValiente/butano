/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_INTRUSIVE_LIST_FWD_H
#define BTN_INTRUSIVE_LIST_FWD_H

/**
 * @file
 * btn::intrusive_list declaration header file.
 *
 * @ingroup intrusive_list
 */

#include "btn_common.h"

namespace btn
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
