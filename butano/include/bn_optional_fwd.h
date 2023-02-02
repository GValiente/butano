/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
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
     * @brief `std::optional` like container.
     *
     * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
     *
     * @tparam Type Element type.
     *
     * @ingroup optional
     */
    template<typename Type>
    class optional;
}

#endif
