/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_REGULAR_BG_ANIMATE_ACTIONS_H
#define BN_REGULAR_BG_ANIMATE_ACTIONS_H

/**
 * @file
 * bn::regular_bg_ptr animate actions declaration header file.
 *
 * @ingroup regular_bg
 * @ingroup bg_map
 * @ingroup action
 */

#include "bn_common.h"

namespace bn
{
    // animation

    class iregular_bg_animate_action;

    /**
     * @brief Changes the map of a regular_bg_ptr when the action is updated a given number of times.
     *
     * Each time the map of a regular_bg_ptr must be changed, it is searched for and created if it has not been found,
     * so the maps are not cached.
     *
     * @tparam MaxSize Maximum number of indexes to maps to store.
     *
     * @ingroup regular_bg
     * @ingroup bg_map
     * @ingroup action
     */
    template<int MaxSize>
    class regular_bg_animate_action;


    // cached animation

    class iregular_bg_cached_animate_action;

    /**
     * @brief Changes the map of a regular_bg_ptr when the action is updated a given number of times.
     *
     * This action differs from regular_bg_animate_action in that all maps to use are cached,
     * so it is faster but takes way more VRAM.
     *
     * @tparam MaxSize Maximum number of maps to store.
     *
     * @ingroup regular_bg
     * @ingroup bg_map
     * @ingroup action
     */
    template<int MaxSize>
    class regular_bg_cached_animate_action;
}

#endif
