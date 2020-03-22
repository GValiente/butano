#ifndef BF_GAME_BACKGROUND_H
#define BF_GAME_BACKGROUND_H

#include "btn_optional.h"
#include "btn_blending_actions.h"
#include "btn_regular_bg_actions.h"
#include "btn_bgs_mosaic_actions.h"

namespace bf
{

class game_background
{

public:
    game_background();

    void show_mosaic();

    void hide_mosaic();

    void reset_blending();

    void update();

private:
    btn::regular_bg_move_by_action _ground_bg_move_action;
    btn::regular_bg_move_by_action _clouds_bg_move_action;
    btn::optional<btn::bgs_mosaic_stretch_loop_action> _mosaic_action;
    btn::optional<btn::blending_transparency_alpha_to_action> _blending_action;
};

}

#endif
