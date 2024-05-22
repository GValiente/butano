/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_ENEMY_BULLET_TYPE_H
#define BF_GAME_ENEMY_BULLET_TYPE_H

#include "bn_common.h"

namespace bf::game
{

enum class enemy_bullet_type : int8_t
{
    SMALL,
    BIG,
    HUGE,
    ROBOT_SMALL,
    CAVEMAN_SMALL,
    CAVEMAN_BIG,
    CAVEMAN_FLIPPED_SMALL,
    CAVEMAN_FLIPPED_BIG,
    ASTRONAUT_SMALL,
    ASTRONAUT_BIG,
    YELLOW_SPACESHIP_SMALL,
    GREEN_SPACESHIP_SMALL,
    BRAIN_SMALL,
    BRAIN_BIG,
    BRAIN_HUGE,
    RED_SPACESHIP_BIG,
    BLUE_SPACESHIP_SMALL,
    BLUE_SPACESHIP_FLIPPED_SMALL,
};

}

#endif
