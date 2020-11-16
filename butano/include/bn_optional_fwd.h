/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_OPTIONAL_FWD_H
#define BTN_OPTIONAL_FWD_H

/**
 * @file
 * btn::optional declaration header file.
 *
 * @ingroup optional
 */

#include "btn_common.h"

namespace btn
{
    /**
     * @brief Manages an optional contained value (a value that may or may not be present).
     *
     * @tparam Type Element type.
     *
     * @ingroup optional
     */
    template<typename Type>
    class optional;
}

#endif
