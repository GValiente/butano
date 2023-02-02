/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HW_BFN_H
#define BN_HW_BFN_H

#include "bn_common.h"

// TONC BFN_* macros without clamping.

//! Get the value of a named bitfield from \a y. Equivalent to (var=) y.name
#define BN_BFN_GET(y, name)     (((y) & name##_MASK) >> name##_SHIFT)

//! Set a named bitfield in \a y to \a x (without clamping). Equivalent to y.name= x
#define BN_BFN_SET(y, x, name)  ((y) = ((y) &~ name##_MASK) | ((x) << name##_SHIFT))


//! Get the value of bitfield \a name from \a y, but don't down-shift
#define BN_BFN_GET_SHIFTED(y, name)     ((y) & name##_MASK)

//! Set bitfield \a name from \a y to \a x with pre-shifted \a x (without clamping)
#define BN_BFN_SET_SHIFTED(y, x, name)  ((y) = ((y) &~ name##_MASK) | (x))

#endif
