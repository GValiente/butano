/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_ARRAY_FWD_H
#define BTN_ARRAY_FWD_H

#include "btn_common.h"

namespace btn
{
    /**
     * @brief Container that encapsulates fixed size arrays.
     *
     * @tparam Type Element type.
     * @tparam Size Number of stored elements.
     *
     * @ingroup array
     */
    template<typename Type, int Size>
    class array;
}

#endif
