/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_common.h"

extern "C"
{
    int memcmp(const void* lhs, const void* rhs, size_t count)
    {
        auto a = static_cast<const char*>(lhs);
        auto b = static_cast<const char*>(rhs);

        for(size_t index = 0; index < count; ++index)
        {
            if(int result = int(a[index]) - int(b[index]))
            {
                return result;
            }
        }

        return 0;
    }
}

