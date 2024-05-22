/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef BF_GAME_BACKGROUND_H
#define BF_GAME_BACKGROUND_H

#include "bn_display.h"
#include "bn_blending_actions.h"
#include "bn_regular_bg_actions.h"
#include "bn_bg_palette_actions.h"
#include "bn_bgs_mosaic_actions.h"
#include "bn_green_swap_actions.h"
#include "bn_regular_bg_position_hbe_ptr.h"

namespace bf::game
{

class stage;

class background
{

public:
    background(const stage& stage, const bn::camera_ptr& camera);

    background(const background& other) = delete;

    background& operator=(const background& other) = delete;

    void set_visible(bool visible);

    void show_bomb_open(int frames);

    void show_bomb_close(int frames);

    void hide_bomb_close(int frames);

    void show_explosion_open(int frames);

    void show_explosion_close(int frames);

    void show_top(int frames);

    void show_hero_dying();

    void show_hero_alive();

    void show_hero_dead();

    void reset();

    void update();

private:
    bn::regular_bg_move_by_action _bottom_move_action;
    bn::regular_bg_move_by_action _top_move_action;
    bn::optional<bn::bgs_mosaic_stretch_loop_action> _mosaic_action;
    bn::optional<bn::blending_transparency_alpha_to_action> _blending_action;
    bn::fixed _hbe_deltas[bn::display::height()];
    bn::regular_bg_position_hbe_ptr _hbe;
    bn::optional<bn::bg_palette_fade_to_action> _bottom_palette_fade_action;
    bn::optional<bn::bg_palette_fade_to_action> _top_palette_fade_action;
    bn::optional<bn::bg_palette_grayscale_to_action> _bottom_palette_grayscale_action;
    bn::optional<bn::bg_palette_inverted_toggle_action> _bottom_palette_inverted_action;
    bn::optional<bn::bg_palette_inverted_toggle_action> _top_palette_inverted_action;
    bn::optional<bn::green_swap_toggle_action> _green_swap_action;
    int _fade_frames = 0;
};

}

#endif
