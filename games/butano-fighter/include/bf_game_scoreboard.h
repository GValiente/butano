/*
 * Copyright (c) 2020 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_SCOREBOARD_H
#define BF_GAME_SCOREBOARD_H

#include "btn_vector.h"
#include "btn_optional.h"
#include "btn_sprite_ptr.h"
#include "btn_sprite_affine_mat_ptr.h"
#include "btn_sprite_palette_actions.h"
#include "btn_sprite_affine_mat_actions.h"
#include "bf_constants.h"

namespace btn
{
    class sprite_text_generator;
}

namespace bf::game
{

class hero;

class scoreboard
{

public:
    explicit scoreboard(btn::sprite_text_generator& text_generator);

    void set_visible(bool visible);

    void update(const hero& hero);

private:
    btn::sprite_text_generator& _text_generator;
    btn::vector<btn::sprite_ptr, 1> _level_label_sprites;
    btn::vector<btn::sprite_ptr, 1> _level_number_sprites;
    btn::vector<btn::sprite_ptr, 1> _experience_label_sprites;
    btn::vector<btn::sprite_ptr, 2> _experience_number_sprites;
    btn::vector<btn::sprite_ptr, 4> _experience_bar_sprites;
    btn::optional<btn::sprite_palette_rotate_by_action> _experience_bar_palette_action;
    btn::vector<btn::sprite_ptr, constants::max_hero_bombs> _bomb_sprites;
    btn::sprite_affine_mat_ptr _bombs_affine_mat;
    btn::optional<btn::sprite_affine_mat_scale_to_action> _bombs_affine_mat_scale_action;
    int _last_level = -1;
    int _last_experience = -1;
    int _last_bombs_count = -1;
};

}

#endif
