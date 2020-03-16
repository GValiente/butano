#ifndef BF_CONSTANTS_H
#define BF_CONSTANTS_H

#include "btn_common.h"

namespace bf::constants
{
    constexpr const int max_hero_bullets = 32;
    constexpr const int max_hero_bombs = 2;
    constexpr const int play_width = 72;
    constexpr const int play_height = (160 - 72) / 2;
    constexpr const int view_width = play_width * 2;
    constexpr const int view_height = (160 + 32) / 2;
    constexpr const int camera_width = 32;
    constexpr const int hero_bullets_z_order = 1;
}

#endif
