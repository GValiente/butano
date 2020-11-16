/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HBLANK_EFFECTS_H
#define BN_HBLANK_EFFECTS_H

/**
 * @file
 * bn::hblank_effects header file.
 *
 * @ingroup hblank_effect
 */

#include "bn_common.h"

/**
 * @brief H-Blank effects related functions.
 *
 * @ingroup hblank_effect
 */
namespace bn::hblank_effects
{
    /**
     * @brief Returns the number of active H-Blank effects.
     */
    [[nodiscard]] int used_count();

    /**
     * @brief Returns the number of available H-Blank effects that can be created.
     */
    [[nodiscard]] int available_count();
}

#endif
