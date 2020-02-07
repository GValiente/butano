#ifndef BF_HERO_BULLETS_H
#define BF_HERO_BULLETS_H

#include "btn_sprite_actions.h"
#include "bf_config.h"

namespace btn
{
    class sound_item;
}

namespace bf
{

class hero;

class hero_bullets
{

public:
    void update(hero& hero);

private:
    btn::vector<btn::sprite_move_by_action, BTN_MAX_HERO_BULLETS> _sprite_move_actions;
    int _b_held_counter = 0;
    int _shoot_event_counter = 0;

    void _remove_bullets();

    void _add_bullets(hero& hero);
};

}

#endif
