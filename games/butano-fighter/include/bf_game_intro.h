/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_INTRO_H
#define BF_GAME_INTRO_H

#include "bn_vector.h"
#include "bn_display.h"
#include "bn_sprite_actions.h"
#include "bn_rect_window_actions.h"
#include "bn_sprite_palette_actions.h"
#include "bn_sprite_third_attributes.h"
#include "bn_sprite_third_attributes_hbe_ptr.h"

namespace bn
{
    class sprite_text_generator;
}

namespace bf
{
    class butano_background;
}

namespace bf::game
{

class stage;

class intro
{

public:
    intro(const stage& stage, bn::sprite_text_generator& text_generator);

    intro(const intro& other) = delete;

    intro& operator=(const intro& other) = delete;

    [[nodiscard]] bool active() const
    {
        return _state != state::DONE;
    }

    void update(const butano_background& butano_background);

private:
    enum class state
    {
        INIT,
        WAIT_1,
        IN,
        WAIT_2,
        WAIT_3,
        OUT,
        DONE
    };

    bn::vector<bn::sprite_ptr, 2> _background_sprites;
    bn::optional<bn::sprite_palette_ptr> _alt_palette;
    bn::vector<bn::sprite_ptr, 40> _text_sprites;
    bn::vector<bn::sprite_move_to_action, 2> _background_sprite_move_actions;
    bn::vector<bn::sprite_vertical_scale_to_action, 2> _background_sprite_vertical_scale_actions;
    bn::vector<bn::sprite_vertical_scale_to_action, 40> _text_sprite_vertical_scale_actions;
    bn::vector<bn::sprite_palette_fade_to_action, 2> _background_sprite_palette_actions;
    bn::vector<bn::sprite_third_attributes, bn::display::height()> _background_sprite_hbe_attributes_1;
    bn::vector<bn::sprite_third_attributes, bn::display::height()> _background_sprite_hbe_attributes_2;
    bn::vector<bn::sprite_third_attributes_hbe_ptr, 2> _background_sprite_hbes;
    bn::optional<bn::rect_window_move_top_to_action> _window_move_top_action;
    bn::optional<bn::rect_window_move_bottom_to_action> _window_move_bottom_action;
    state _state = state::INIT;
    int _counter = -1;
};

}

#endif
