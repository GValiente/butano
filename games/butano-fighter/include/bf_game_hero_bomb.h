#ifndef BF_GAME_HERO_BOMB_H
#define BF_GAME_HERO_BOMB_H

#include "btn_optional.h"
#include "btn_bgs_mosaic_actions.h"
#include "btn_regular_bg_actions.h"
#include "btn_rect_window_actions.h"

namespace bf
{

class game_hero;

class game_hero_bomb
{

public:
    game_hero_bomb();

    game_hero_bomb(const game_hero_bomb& other) = delete;

    game_hero_bomb& operator=(const game_hero_bomb& other) = delete;

    void update(game_hero& hero);

private:
    enum class status_type
    {
        INACTIVE,
        OPEN,
        CLOSE
    };

    btn::regular_bg_ptr _bg;
    btn::regular_bg_move_by_action _bg_move_action;
    btn::optional<btn::rect_window_move_top_left_to_action> _move_top_left_window_action;
    btn::optional<btn::rect_window_move_bottom_right_to_action> _move_bottom_right_window_action;
    btn::bgs_mosaic_stretch_loop_action _bgs_mosaic_action;
    btn::fixed _old_blending_transparency_alpha;
    status_type _status = status_type::INACTIVE;
    int _counter = 0;
};

}

#endif
