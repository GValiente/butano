#ifndef BF_GAME_HERO_BOMB_H
#define BF_GAME_HERO_BOMB_H

#include "btn_optional.h"
#include "btn_blending_actions.h"
#include "btn_regular_bg_actions.h"
#include "btn_rect_window_actions.h"
#include "btn_regular_bg_hblank_effects.h"
#include "btn_rect_window_hblank_effects.h"
#include "bf_circle_generator.h"

namespace bf::game
{

class hero;
class background;

class hero_bomb
{

public:
    hero_bomb();

    hero_bomb(const hero_bomb& other) = delete;

    hero_bomb& operator=(const hero_bomb& other) = delete;

    void update(hero& hero, background& background);

private:
    enum class status_type
    {
        INACTIVE,
        OPEN,
        CLOSE
    };

    btn::regular_bg_ptr _bg;
    btn::regular_bg_move_by_action _bg_move_action;
    btn::optional<btn::rect_window_move_top_by_action> _move_window_top_action;
    btn::optional<btn::rect_window_move_bottom_by_action> _move_window_bottom_action;
    btn::optional<btn::blending_transparency_alpha_to_action> _blending_action;
    btn::pair<btn::fixed, btn::fixed> _circle_hblank_effect_deltas[160];
    circle_generator _circle_generator;
    btn::rect_window_boundaries_hblank_effect_ptr _circle_hblank_effect;
    btn::fixed _wave_hblank_effect_deltas[160];
    btn::regular_bg_position_hblank_effect_ptr _wave_hblank_effect;
    status_type _status = status_type::INACTIVE;
    int _counter = 0;
    int _flame_sound_counter = 0;

    void _play_flame_sound();
};

}

#endif
