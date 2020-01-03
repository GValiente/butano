#ifndef BTN_LIMITS_H
#define BTN_LIMITS_H

#include "../3rd_party/etl/include/etl/integral_limits.h"

namespace btn
{
    template<typename Type>
    using integral_limits = etl::integral_limits<Type>;
}

#endif
