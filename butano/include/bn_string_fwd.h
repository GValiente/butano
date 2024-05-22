/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_STRING_FWD_H
#define BN_STRING_FWD_H

/**
 * @file
 * bn::istring and bn::string declaration header file.
 *
 * @ingroup string
 */

#include "bn_common.h"

namespace bn
{
    class istring_base;

    class istring;

    /**
     * @brief `std::string` like container with a fixed size buffer.
     *
     * It doesn't throw exceptions. Instead, asserts are used to ensure valid usage.
     *
     * @tparam MaxSize Maximum number of characters that can be stored.
     *
     * @ingroup string
     */
    template<int MaxSize>
    class string;
}

#endif
