/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_CORE_UPDATE_CALLBACK_TYPE_H
#define BN_CORE_UPDATE_CALLBACK_TYPE_H

/**
 * @file
 * bn::core::update_callback_type header file.
 *
 * @ingroup core
 */

#include "bn_common.h"

namespace bn::core
{
    using update_callback_type = void(*)(); //!< Core update callback type alias.
}

#endif
