/*
 * Copyright (c) 2020-2025 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_HBES_H
#define BN_HBES_H

/**
 * @file
 * bn::hbes header file.
 *
 * @ingroup hblank_effect
 */

#include "bn_common.h"

/**
 * @brief H-Blank effects related functions.
 *
 * @ingroup hblank_effect
 */
namespace bn::hbes
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
