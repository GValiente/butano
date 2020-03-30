#ifndef BF_GAME_BACKGROUND_H
#define BF_GAME_BACKGROUND_H

#include "btn_optional.h"
#include "btn_blending_actions.h"
#include "btn_regular_bg_actions.h"
#include "btn_bg_palette_actions.h"
#include "btn_bgs_mosaic_actions.h"
#include "btn_regular_bg_hblank_effects.h"
#include "bf_wave_generator.h"

namespace bf::game
{

class background
{

public:
    background();

    background(const background& other) = delete;

    background& operator=(const background& other) = delete;

    void show_bomb_open(int frames);

    void show_bomb_fade(int frames);

    void show_clouds();

    void update();

private:
    btn::regular_bg_move_by_action _ground_move_action;
    btn::regular_bg_move_by_action _clouds_move_action;
    btn::optional<btn::bgs_mosaic_stretch_loop_action> _mosaic_action;
    btn::optional<btn::blending_transparency_alpha_to_action> _blending_action;
    btn::fixed _hblank_effect_deltas[160];
    btn::regular_bg_position_hblank_effect_ptr _hblank_effect;
    wave_generator _wave_generator;
    btn::optional<btn::bg_palette_grayscale_to_action> _ground_palette_action;
    int _bomb_fade_frames = 0;
};

}

#endif
