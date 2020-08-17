#ifndef BF_GAME_ENEMY_BULLET_TYPE_H
#define BF_GAME_ENEMY_BULLET_TYPE_H

#include "btn_common.h"

namespace bf::game
{

enum class enemy_bullet_type : int8_t
{
    SMALL,
    BIG,
    HUGE,
    ROBOT_DOUBLE,
    CAVEMAN_SMALL,
    CAVEMAN_BIG,
};

}

#endif
