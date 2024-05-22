/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_ENEMY_DROP_TYPE_H
#define BF_GAME_ENEMY_DROP_TYPE_H

#include "bn_common.h"

namespace bf::game
{

enum class enemy_drop_type : int8_t
{
    NONE,
    GEM,
    HERO_BOMB
};

}

#endif
