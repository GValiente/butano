/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_VBLANK_CALLBACK_TYPE_H
#define BN_VBLANK_CALLBACK_TYPE_H

/**
 * @file
 * bn::vblank_callback_type header file.
 *
 * @ingroup core
 */

#include "bn_common.h"

namespace bn
{
    using vblank_callback_type = void(*)(); //!< V-Blank callback type alias.
}

#endif
