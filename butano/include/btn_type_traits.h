#ifndef BTN_TYPE_TRAITS_H
#define BTN_TYPE_TRAITS_H

#include <type_traits>
#include "btn_common.h"

namespace btn
{
    using std::remove_reference;

    using std::is_same;

    using std::is_same_v;

    using std::is_trivial;

    using std::is_trivially_copyable;

    using std::decay;
}

#endif
