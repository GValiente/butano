/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_HERO_BULLETS_H
#define BF_GAME_HERO_BULLETS_H

#include "bn_array.h"
#include "bn_forward_list.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_palette_ptr.h"
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

    void update(const bn::camera_ptr& camera, hero& hero, enemies& enemies, objects& objects);

private:
    struct bullet_type
    {
        bn::sprite_move_by_action sprite_move_action;
        const hero_bullet_level* level_data;
    };

    bn::sprite_palette_ptr _palette;
    bn::array<bn::sprite_tiles_ptr, 9> _tiles;
    bn::forward_list<bullet_type, constants::max_hero_bullets / 2> _even_bullets;
    bn::forward_list<bullet_type, constants::max_hero_bullets / 2> _odd_bullets;
    int _b_held_counter = 0;
    int _event_counter = 0;
    bool _check_odds = false;

    void _remove_bullets(const bn::camera_ptr& camera, hero& hero, enemies& enemies, objects& objects);

    void _add_bullets(const bn::camera_ptr& camera, hero& hero);
};

}

#endif
