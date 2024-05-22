/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_CHECK_HERO_BULLET_DATA_H
#define BF_GAME_CHECK_HERO_BULLET_DATA_H

#include "bn_fixed_rect.h"

namespace bn
{
    class camera_ptr;
}

namespace bf::game
{

class hero;
class objects;

class check_hero_bullet_data
{

public:
    bn::fixed_rect bullet_rect;
    int bullet_damage;
    const bn::camera_ptr& camera_ref;
    hero& hero_ref;
    objects& objects_ref;
};

}

#endif
