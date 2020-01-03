#ifndef BTN_STRING_FWD_H
#define BTN_STRING_FWD_H

#include "btn_common.h"

namespace etl
{
    template<typename Type>
    class ibasic_string;

    using istring = ibasic_string<char>;

    template<const size_t MaxSize>
    class string;
}

namespace btn
{
    using istring = etl::istring;

    template<size_t MaxSize>
    using string = etl::string<MaxSize>;
}

#endif
