#ifndef BTN_ARRAY_FWD_H
#define BTN_ARRAY_FWD_H

#include "btn_common.h"

namespace etl
{
    template<typename Type, const size_t Size>
    class array;
}

namespace btn
{
    template<typename Type, size_t Size>
    using array = etl::array<Type, Size>;
}

#endif
