#ifndef BTN_POOL_FWD_H
#define BTN_POOL_FWD_H

#include "btn_common.h"

namespace etl
{
    class ipool;

    template<typename Type, const size_t MaxSize>
    class pool;
}

namespace btn
{
    using ipool = etl::ipool;

    template<typename Type, size_t MaxSize>
    using pool = etl::pool<Type, MaxSize>;
}

#endif
