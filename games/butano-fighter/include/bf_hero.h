#ifndef BF_HERO_H
#define BF_HERO_H

#include "btn_sprite_actions.h"

namespace bf
{

class hero_bullets;

class hero
{

public:
    hero();

    const btn::fixed_point& position();

    void update(hero_bullets& bullets);

private:
    int _level = 0;
    int _counter = 0;
    btn::sprite_cached_animate_action<2> _body_sprite_animate_action;
    btn::sprite_ptr _weapon_sprite;
};

}

#endif
