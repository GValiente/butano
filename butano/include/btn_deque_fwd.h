#ifndef BTN_DEQUE_FWD_H
#define BTN_DEQUE_FWD_H

#include "btn_common.h"

namespace etl
{
    template<typename Type>
    class ideque;

    template<typename Type, const size_t MaxSize>
    class deque;
}

namespace btn
{
    template<typename Type>
    using ideque = etl::ideque<Type>;

    template<typename Type, size_t MaxSize>
    using deque = etl::deque<Type, MaxSize>;
}

#endif
