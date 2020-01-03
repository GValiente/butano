#ifndef BTN_VECTOR_FWD_H
#define BTN_VECTOR_FWD_H

#include "btn_common.h"

namespace etl
{
    template<typename Type>
    class ivector;

    template<typename Type, const size_t MaxSize>
    class vector;
}

namespace btn
{
    template<typename Type>
    using ivector = etl::ivector<Type>;

    template<typename Type, size_t MaxSize>
    using vector = etl::vector<Type, MaxSize>;
}

#endif
