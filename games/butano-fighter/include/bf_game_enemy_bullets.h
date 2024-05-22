/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_ENEMY_BULLETS_H
#define BF_GAME_ENEMY_BULLETS_H

#include "bn_array.h"
#include "bn_forward_list.h"
#include "bn_fixed_rect_fwd.h"
#include "bn_sprite_actions.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_palette_actions.h"
#include "bf_constants.h"
#include "bf_game_enemy_bullet_type.h"

namespace bf::game
{

class enemy_bullet_event;

class enemy_bullets
{

public:
    enemy_bullets();

    [[nodiscard]] bool check_hero(const bn::fixed_rect& hero_rect);

    void add_bullet(const bn::fixed_point& hero_position, const bn::fixed_point& enemy_position,
                    const enemy_bullet_event& event, const bn::camera_ptr& camera);

    void clear();

    void update();

private:
    struct bullet_type
    {
        bn::sprite_move_by_action sprite_move_action;
        bn::optional<bn::sprite_rotate_by_action> sprite_rotate_action;
        enemy_bullet_type type;
    };

    bn::sprite_palette_fade_loop_action _palette_fade_action;
    bn::array<bn::sprite_tiles_ptr, 2> _tiles_list;
    bn::forward_list<bullet_type, constants::max_enemy_bullets / 2> _even_bullets;
    bn::forward_list<bullet_type, constants::max_enemy_bullets / 2> _odd_bullets;
    bool _check_odds = false;
};

}

#endif
