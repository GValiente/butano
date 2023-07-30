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

        if(a != b)
        {
            while(count)
            {
                if(int result = int(*a) - int(*b))
                {
                    return result;
                }

                ++a;
                ++b;
                --count;
            }
        }

        return 0;
    }
}

