#ifndef BTN_TYPE_TRAITS_H
#define BTN_TYPE_TRAITS_H

#include "../3rd_party/etl/include/etl/type_traits.h"

namespace btn
{
    using etl::remove_reference;

    using std::is_trivial;

    using std::is_trivially_copyable;
}

#endif
