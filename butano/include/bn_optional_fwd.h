/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_OPTIONAL_FWD_H
#define BN_OPTIONAL_FWD_H

/**
 * @file
 * bn::optional declaration header file.
 *
 * @ingroup optional
 */

#include "bn_common.h"

namespace bn
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
