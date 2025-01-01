/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_IDENTITY_HASHER_H
#define BN_IDENTITY_HASHER_H

#include "bn_common.h"

namespace bn
{

class identity_hasher
{

public:
    template<typename Type>
    [[nodiscard]] constexpr unsigned operator()(const Type& value) const
    {
        return unsigned(value);
    }
};

}

#endif
