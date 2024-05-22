/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_SCOREBOARD_H
#define BF_GAME_SCOREBOARD_H

#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_affine_mat_ptr.h"
#include "bn_sprite_palette_actions.h"
#include "bn_sprite_affine_mat_actions.h"
#include "bf_constants.h"

namespace bn
{
    class sprite_text_generator;
}

namespace bf::game
{

class hero;

class scoreboard
{

public:
    explicit scoreboard(bn::sprite_text_generator& text_generator);

    void set_visible(bool visible);

    void update(const hero& hero);

private:
    bn::sprite_text_generator& _text_generator;
    bn::vector<bn::sprite_ptr, 1> _level_label_sprites;
    bn::vector<bn::sprite_ptr, 1> _level_number_sprites;
    bn::vector<bn::sprite_ptr, 1> _experience_label_sprites;
    bn::vector<bn::sprite_ptr, 2> _experience_number_sprites;
    bn::vector<bn::sprite_ptr, 4> _experience_bar_sprites;
    bn::optional<bn::sprite_palette_rotate_by_action> _experience_bar_palette_action;
    bn::vector<bn::sprite_ptr, constants::max_hero_bombs> _bomb_sprites;
    bn::sprite_affine_mat_ptr _bombs_affine_mat;
    bn::optional<bn::sprite_affine_mat_scale_to_action> _bombs_affine_mat_scale_action;
    int _last_level = -1;
    int _last_experience = -1;
    int _last_bombs_count = -1;
};

}

#endif
