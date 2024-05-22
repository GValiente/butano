/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_common.h"

extern "C"
{

int memcmp(const void* lhs, const void* rhs, size_t count)
{
    auto a = static_cast<const char*>(lhs);
    auto b = static_cast<const char*>(rhs);

    if(count >= 4) [[likely]]
    {
        uintptr_t alignment = uintptr_t(a) % 4;

        if(alignment == uintptr_t(b) % 4) [[likely]]
        {
            while(alignment && count)
            {
                if(int result = int(*a) - int(*b))
                {
                    return result;
                }

                ++a;
                ++b;
                --alignment;
                --count;
            }

            while(count >= 4)
            {
                auto a_int = reinterpret_cast<const int*>(a);
                auto b_int = reinterpret_cast<const int*>(b);

                if(int result = *a_int - *b_int)
                {
                    return result;
                }

                a += 4;
                b += 4;
                count -= 4;
            }
        }
    }

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

    return 0;
}

}
