/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BTN_HBLANK_EFFECTS_H
#define BTN_HBLANK_EFFECTS_H

/**
 * @file
 * btn::hblank_effects header file.
 *
 * @ingroup hblank_effect
 */

#include "btn_common.h"

/**
 * @brief H-Blank effects related functions.
 *
 * @ingroup hblank_effect
 */
namespace btn::hblank_effects
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
