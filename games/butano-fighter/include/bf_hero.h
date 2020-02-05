#ifndef BF_HERO_H
#define BF_HERO_H

#include "btn_sprite_actions.h"

namespace bf
{

class hero
{

public:
    hero();

    [[nodiscard]] int level() const
    {
        return _level;
    }

    [[nodiscard]] const btn::fixed_point& weapon_position() const
    {
        return _weapon_sprite.position();
    }

    void update();

private:
    int _level = 2;
    btn::sprite_cached_animate_action<2> _body_sprite_animate_action;
    btn::sprite_ptr _weapon_sprite;
};

}

#endif
