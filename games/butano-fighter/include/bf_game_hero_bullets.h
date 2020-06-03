#ifndef BF_GAME_HERO_BULLETS_H
#define BF_GAME_HERO_BULLETS_H

#include "btn_forward_list.h"
#include "btn_sprite_actions.h"
#include "btn_sprite_palette_ptr.h"
#include "bf_constants.h"

namespace bf::game
{

class hero;
class enemies;
class objects;
class hero_bullet_level;

class hero_bullets
{

public:
    hero_bullets();

    void update(hero& hero, enemies& enemies, objects& objects);

private:
    struct bullet_type
    {
        btn::sprite_move_by_action sprite_move_action;
        const hero_bullet_level* level_data;
    };

    btn::sprite_palette_ptr _palette;
    btn::array<btn::sprite_tiles_ptr, 9> _tiles;
    btn::forward_list<bullet_type, constants::max_hero_bullets / 2> _even_bullets;
    btn::forward_list<bullet_type, constants::max_hero_bullets / 2> _odd_bullets;
    int _b_held_counter = 0;
    int _event_counter = 0;
    bool _check_odds = false;

    void _remove_bullets(hero& hero, enemies& enemies, objects& objects);

    void _add_bullets(hero& hero);
};

}

#endif
