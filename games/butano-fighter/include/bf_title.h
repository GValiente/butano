/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_TITLE_H
#define BF_TITLE_H

#include "bn_array.h"
#include "bn_vector.h"
#include "bn_display.h"
#include "bn_music_actions.h"
#include "bn_sprite_actions.h"
#include "bn_blending_actions.h"
#include "bn_sprite_position_hbe_ptr.h"
#include "bn_sprite_affine_mat_attributes_hbe_ptr.h"
#include "bf_scene.h"

namespace bn
{
    class sprite_text_generator;
}

namespace bf
{

class status;
class butano_background;

class title : public scene
{

public:
    title(status& status, bn::sprite_text_generator& text_generator, butano_background& butano_background);

    title(const title& other) = delete;

    title& operator=(const title& other) = delete;

    [[nodiscard]] bn::optional<scene_type> update() final;

private:
    enum class state
    {
        START,
        BUTANO_CHARACTERS_SHOW,
        FIGHTER_CHARACTERS_SHOW,
        MENU,
        HIDE_CURSOR,
        HIDE_SPRITES,
        END
    };

    status& _status;
    bn::sprite_text_generator& _text_generator;
    bn::sprite_ptr _butano_up_sprite;
    bn::sprite_ptr _butano_down_sprite;
    bn::vector<bn::sprite_ptr, 6> _butano_characters;
    bn::vector<bn::sprite_ptr, 7> _fighter_characters;
    bn::vector<bn::sprite_ptr, 4> _high_experience_text_sprites;
    bn::vector<bn::sprite_ptr, 2> _start_text_sprites;
    bn::vector<bn::sprite_ptr, 3> _how_to_play_sprites;
    bn::vector<bn::sprite_ptr, 2> _credits_text_sprites;
    bn::vector<bn::sprite_ptr, 3> _rumble_text_sprites;
    bn::sprite_ptr _cursor_sprite;
    bn::fixed _butano_y_inc = -4.05;
    bn::array<bn::fixed, bn::display::height()> _butano_x_hbe_deltas;
    bn::optional<bn::sprite_position_hbe_ptr> _butano_up_x_hbe;
    bn::optional<bn::sprite_position_hbe_ptr> _butano_down_x_hbe;
    bn::optional<bn::sprite_move_to_action> _butano_character_move_action;
    bn::optional<bn::sprite_position_hbe_ptr> _butano_character_hbe;
    bn::optional<bn::sprite_move_to_action> _fighter_character_move_action;
    bn::optional<bn::sprite_move_to_action> _cursor_move_action;
    bn::optional<bn::sprite_scale_to_action> _cursor_scale_action;
    bn::optional<bn::music_volume_to_action> _music_volume_action;
    bn::optional<bn::blending_intensity_alpha_to_action> _blending_intensity_action;
    bn::optional<bn::blending_transparency_alpha_to_action> _blending_transparency_action;
    state _state = state::START;
    int _menu_index = 0;
    int _butano_x_hbe_speed = 64 * 32;
    bn::optional<bn::sprite_affine_mat_attributes_hbe_ptr> _fighter_character_hbe;
    bool _butano_y_up = true;

    void _animate_butano_x();

    void _animate_butano_y();

    void _animate_butano_characters();

    void _animate_fighter_characters();

    bn::optional<scene_type> _menu();
};

}

#endif
