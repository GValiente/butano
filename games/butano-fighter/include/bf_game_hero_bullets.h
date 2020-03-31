#ifndef BF_GAME_HERO_BULLETS_H
#define BF_GAME_HERO_BULLETS_H

#include "btn_sprite_actions.h"
#include "bf_constants.h"

namespace btn
{
    class sound_item;
}

namespace bf::game
{

class hero;
class enemies;
class hero_bullet_level;

class hero_bullets
{

public:
    void update(hero& hero, enemies& enemies);

private:
    struct bullet
    {
        btn::sprite_move_by_action sprite_move_action;
        const hero_bullet_level* level_data;
    };

    btn::vector<bullet, constants::max_hero_bullets> _bullets;
    int _b_held_counter = 0;
    int _event_counter = 0;

    void _remove_bullets(enemies& enemies);

    void _add_bullets(hero& hero);
};

}

#endif
