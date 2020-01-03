#ifndef BTN_LIST_FWD_H
#define BTN_LIST_FWD_H

#include "btn_common.h"

namespace etl
{
    template<typename Type>
    class ilist;

    template<typename Type, const size_t MaxSize>
    class list;
}

namespace btn
{
    template<typename Type>
    using ilist = etl::ilist<Type>;

    template<typename Type, size_t MaxSize>
    using list = etl::list<Type, MaxSize>;
}

#endif
