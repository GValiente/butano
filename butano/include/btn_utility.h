#ifndef BTN_UTILITY_H
#define BTN_UTILITY_H

#include <utility>
#include "btn_common.h"

namespace btn
{
    using std::move;

    using std::forward;

    using std::pair;

    using std::make_pair;

    template<typename Type>
    constexpr void swap(Type& a, Type& b)
    {
        if(BTN_CONSTANT_EVALUATED())
        {
            Type temp = move(a);
            a = move(b);
            b = move(temp);
        }
        else
        {
            std::swap(a, b);
        }
    }
}

#endif
