/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BN_SPRITE_ANIMATE_ACTIONS_H
#define BN_SPRITE_ANIMATE_ACTIONS_H

/**
 * @file
 * bn::sprite_ptr animate actions declaration header file.
 *
 * @ingroup sprite
 * @ingroup tile
 * @ingroup action
 */

#include "bn_common.h"

namespace bn
{
    // animation

    class isprite_animate_action;

    /**
     * @brief Changes the tile set of a sprite_ptr when the action is updated a given number of times.
     *
     * Each time the tile set of a sprite_ptr must be changed, it is searched for and created if it has not been found,
     * so the tile sets are not cached.
     *
     * @tparam MaxSize Maximum number of indexes to sprite tile sets to store.
     *
     * @ingroup sprite
     * @ingroup tile
     * @ingroup action
     */
    template<int MaxSize>
    class sprite_animate_action;


    // cached animation

    class isprite_cached_animate_action;

    /**
     * @brief Changes the tile set of a sprite_ptr when the action is updated a given number of times.
     *
     * This action differs from sprite_animate_action in that all sprite tile sets to use are cached,
     * so it is faster but takes way more VRAM.
     *
     * @tparam MaxSize Maximum number of sprite tile sets to store.
     *
     * @ingroup sprite
     * @ingroup tile
     * @ingroup action
     */
    template<int MaxSize>
    class sprite_cached_animate_action;
}

#endif
