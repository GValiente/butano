#ifndef BF_GAME_BACKGROUND_H
#define BF_GAME_BACKGROUND_H

#include "btn_display.h"
#include "btn_optional.h"
#include "btn_blending_actions.h"
#include "btn_regular_bg_actions.h"
#include "btn_bg_palette_actions.h"
#include "btn_bgs_mosaic_actions.h"
#include "btn_green_swap_actions.h"
#include "btn_regular_bg_hblank_effects.h"

namespace bf::game
{

class stage;

class background
{

public:
    explicit background(const stage& stage);

    background(const background& other) = delete;

    background& operator=(const background& other) = delete;

    void show_bomb_open(int frames);

    void show_bomb_close(int frames);

    void hide_bomb_close(int frames);

    void show_top(int frames);

    void show_hero_dying();

    void show_hero_alive();

    void show_hero_dead();

    void reset();

    void update();

private:
    btn::regular_bg_move_by_action _bottom_move_action;
    btn::regular_bg_move_by_action _top_move_action;
    btn::optional<btn::bgs_mosaic_stretch_loop_action> _mosaic_action;
    btn::optional<btn::blending_transparency_alpha_to_action> _blending_action;
    btn::fixed _hblank_effect_deltas[btn::display::height()];
    btn::regular_bg_position_hblank_effect_ptr _hblank_effect;
    btn::optional<btn::bg_palette_grayscale_to_action> _bottom_palette_grayscale_action;
    btn::optional<btn::bg_palette_inverted_toggle_action> _bottom_palette_inverted_action;
    btn::optional<btn::bg_palette_inverted_toggle_action> _top_palette_inverted_action;
    btn::optional<btn::green_swap_toggle_action> _green_swap_action;
    int _bomb_fade_frames = 0;
};

}

#endif
