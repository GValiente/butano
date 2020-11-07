/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_TYPE_TRAITS_H
#define BTN_TYPE_TRAITS_H

#include <type_traits>
#include "btn_common.h"

namespace btn
{
    using std::remove_reference;
    using std::remove_reference_t;

    using std::is_same;
    using std::is_same_v;

    using std::is_trivial;
    using std::is_trivial_v;

    using std::is_trivially_copyable;
    using std::is_trivially_copyable_v;

    using std::is_copy_constructible;
    using std::is_copy_constructible_v;

    using std::is_copy_assignable;
    using std::is_copy_assignable_v;

    using std::is_move_constructible;
    using std::is_move_constructible_v;

    using std::is_move_assignable;
    using std::is_move_assignable_v;

    using std::is_swappable;
    using std::is_swappable_v;

    using std::is_base_of;
    using std::is_base_of_v;

    using std::decay;
    using std::decay_t;

    using std::remove_cv;
    using std::remove_cv_t;

    using std::is_constant_evaluated;
}

#endif
