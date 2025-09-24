/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_PALETTE_EFFECT_TYPE_H
#define BN_PALETTE_EFFECT_TYPE_H

/**
 * @file
 * bn::palette_effect_type header file.
 *
 * @ingroup palette
 */

#include "bn_span.h"

namespace bn
{
    class color;

    using palette_effect_type = void(*)(span<color>); //!< Palette effect type alias.
}

#endif
