#ifndef BTN_ARRAY_FWD_H
#define BTN_ARRAY_FWD_H

#include "btn_common.h"

namespace etl
{
    class ibitset;

    template<const size_t Size>
    class bitset;
}

namespace btn
{
    using ibitset = etl::ibitset;

    template<size_t Size>
    using bitset = etl::bitset<Size>;
}

#endif
