#ifndef BTN_FORWARD_LIST_FWD_H
#define BTN_FORWARD_LIST_FWD_H

#include "btn_common.h"

namespace etl
{
    template<typename Type>
    class iforward_list;

    template<typename Type, const size_t MaxSize>
    class forward_list;
}

namespace btn
{
    template<typename Type>
    using iforward_list = etl::iforward_list<Type>;

    template<typename Type, size_t MaxSize>
    using forward_list = etl::forward_list<Type, MaxSize>;
}

#endif
