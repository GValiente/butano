#ifndef BF_GAME_CHECK_HERO_BULLET_DATA_H
#define BF_GAME_CHECK_HERO_BULLET_DATA_H

#include "btn_fixed_rect.h"

namespace btn
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
    btn::fixed_rect bullet_rect;
    int bullet_damage;
    const btn::camera_ptr& camera_ref;
    hero& hero_ref;
    objects& objects_ref;
};

}

#endif
