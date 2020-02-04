#ifndef BF_HERO_BULLETS_H
#define BF_HERO_BULLETS_H

#include "btn_sprite_actions.h"
#include "bf_config.h"

namespace bf
{

class hero_bullets
{

public:
    void add(int level, const btn::fixed_point& position);

    void update();

private:
    btn::vector<btn::sprite_move_by_action, BTN_MAX_HERO_BULLETS> _sprite_move_actions;
};

}

#endif
