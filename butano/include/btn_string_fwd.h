/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_STRING_FWD_H
#define BTN_STRING_FWD_H

#include "btn_common.h"

namespace btn
{
    /**
     * @brief Base class of istring.
     *
     * It avoids circular references when including istring's header.
     *
     * @ingroup string
     */
    class istring_base;

    /**
     * @brief Base class of string.
     *
     * Can be used as a reference type for all string containers.
     *
     * @ingroup string
     */
    class istring;

    /**
     * @brief String implementation that uses a fixed size buffer.
     *
     * @tparam MaxSize Maximum number of characters that can be stored.
     *
     * @ingroup string
     */
    template<int MaxSize>
    class string;
}

#endif
