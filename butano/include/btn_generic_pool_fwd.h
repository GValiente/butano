#ifndef BTN_GENERIC_POOL_FWD_H
#define BTN_GENERIC_POOL_FWD_H

#include "btn_common.h"

namespace btn
{
    template<int TypeSize>
    class igeneric_pool;

    template<int TypeSize, int TypeAlignment, int MaxSize>
    class generic_pool;
}

#endif
